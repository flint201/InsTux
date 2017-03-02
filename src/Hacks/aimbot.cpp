#include "aimbot.h"
//#include "autowall.h"
#include "../interfaces.h"
#include "../Utils/mousesim.h"
#include <math.h>
#include "hacks.h"
#include "../settings.h"

#define NUM_BONE 128

float Settings::Aimbot::k = 2;

float silentFov = 5;
float silentHipFov = 30;
float forceSelectFov = 1;

namespace Aimbot
{
    int targetIdx = -1;
    Vector lastAimPoint;
}


C_BasePlayer* GetClosestPlayer(CUserCmd* cmd, C_BasePlayer* localplayer, Bone& bestBone, Vector& aimPoint, bool locked, QAngle& angSilent)
{
    TeamID myteam = localplayer->GetTeam();
    Vector eyePos = localplayer->GetEyePosition();

    C_BasePlayer* closestEntity = NULL;
    float bestFov = 10;

    for (int i = 0; i <= engine->GetMaxClients(); i++)
    {
        C_BasePlayer* player = (C_BasePlayer*) entityList->GetClientEntity(i);
        if (!player
            || player == localplayer
            || player->GetTeam() == myteam
            || player->GetDormant()
            || !player->GetAlive())
        {
            continue;
        }

        if (locked
                && i != Aimbot::targetIdx
                && Math::GetDistance(Aimbot::lastAimPoint, player->GetEyePosition()) > 12*10) // 12 feet
        {
            continue;
        }


        Vector vecCenterMass = player->GetEyePosition();
        float fov = Math::GetFov(cmd->viewangles, Math::CalcAngle(eyePos, vecCenterMass));

        if (fov > bestFov)
            continue;

        Bone targetBones[] = { Bone::SPINE_3, Bone::HEAD, Bone::ELBOW_L, Bone::ELBOW_R, Bone::KNEE_L, Bone::KNEE_R };
        static matrix3x4_t BoneMatrix[NUM_BONE];
        player->SetupBones(BoneMatrix, NUM_BONE, BONE_USED_BY_HITBOX, 0);

        // for silent aim targeting
        matrix3x4_t boneHead = BoneMatrix[(int)Bone::HEAD];
        Vector vecBoneHead = Vector(boneHead[0][3], boneHead[1][3], boneHead[2][3]);
        if (Util::Ray(localplayer, player, i, eyePos, vecBoneHead))
        {
            QAngle angHead = Math::CalcAngle(eyePos, vecBoneHead);
            if (Math::GetFov(cmd->viewangles, angHead) < silentFov ||
                (Math::GetFov(cmd->muzzleangle, angHead) < silentHipFov && Math::GetFov(cmd->viewangles, cmd->muzzleangle) > 5))
            {
                angSilent = angHead;
            }
        }

        // find best bone for legit aim
        bool found = false;
        for (Bone b : targetBones)
        {
            matrix3x4_t currBone = BoneMatrix[(int)b];
            Vector vecBone = Vector(currBone[0][3], currBone[1][3], currBone[2][3]);
            
            /*
            Vector vecEnd = vecBone;
            Ray_t ray;
            ray.Init(localplayer->GetEyePosition(), vecEnd);

            CTraceFilter traceFilter;
            traceFilter.pSkip = localplayer;

            trace_t tr;
            trace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

            Log << "    ---- trace vec end, bone = " << vecEnd.x << " " << vecEnd.y << " " << vecEnd.z << " " << (int)b << endl;
            Log << "    player = " << (unsigned) player << endl;
            Log << "    playerIdx = " << (unsigned) i << endl;
            Log << "    tr.fraction = " << tr.fraction << endl;
            Log << "    tr.contents = " << tr.contents << endl;
            Log << "    tr.dispFlags = " << tr.dispFlags << endl;
            Log << "    tr.allsolid = " << tr.allsolid << endl;
            Log << "    tr.startsolid = " << tr.startsolid << endl;
            Log << "    tr.fractionleftsolid = " << tr.fractionleftsolid << endl;
            Log << "      tr.surface.name = " << (unsigned)tr.surface.name << endl;
            Log << "      tr.surface.surfaceProps = " << tr.surface.surfaceProps << endl;
            Log << "      tr.surface.flags = " << tr.surface.flags << endl;
            Log << "    tr.hitgroup = " << (unsigned) tr.hitgroup <<  endl;
            Log << "    tr.physicsbone = " << (unsigned) tr.physicsbone <<  endl;
            Log << "    tr.worldSurfaceIndex = " << (unsigned) tr.worldSurfaceIndex <<  endl;
            Log << "    tr.m_pEntityHit = " << (unsigned) tr.m_pEntityHit <<  endl;
            Log << "    tr.hitbox = " << (unsigned) tr.hitbox <<  endl;
            for (int i = -16; i < 8; i++)
            {
                Log << "  " << *(unsigned*)((uintptr_t)(&tr.m_pEntityHit) + i) << endl;
            }
            //*/

            if (!Util::Ray(localplayer, player, i, eyePos, vecBone))
            {
                continue;
            }

            Aimbot::targetIdx = i;
            closestEntity = player;
            bestFov = fov;
            bestBone = b;
            aimPoint = vecBone;
            found = true;
            break;
        }

        if (!found && fov < forceSelectFov)
        {
            Aimbot::targetIdx = i;
            closestEntity = player;
            bestFov = fov;
            bestBone = Bone::HEAD;
            matrix3x4_t currBone = BoneMatrix[(int)bestBone];
            aimPoint = Vector(currBone[0][3], currBone[1][3], currBone[2][3]);
        }
    }

    return closestEntity;
}


long GetMS()
{
    static struct timespec spec;
    if (clock_gettime(CLOCK_REALTIME, &spec) < 0)
        return 0;
    return spec.tv_sec*1000 + spec.tv_nsec/1000;
}


long GetDT(long min = 5, long max = 20)
{
    static long ms = GetMS();
    long dt = GetMS() - ms;
    if (dt < min) return min;
    if (dt > max) return max;
    return dt;
}


void Aimbot::CreateMove(CUserCmd* cmd)
{
    MouseSim::update(cmd);

    bool aimKeyDown = inputSystem->IsButtonDown(ButtonCode_t::KEY_LSHIFT);

    QAngle angle = cmd->viewangles;
    C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

    if (!localplayer || !localplayer->GetAlive())
    {
        return;
    }

    if (!(localplayer->GetFlags() & FL_ONGROUND))
    {
        return;
    }

    Bone bone = Bone::SPINE_3;
    Vector aimPoint;
    QAngle angSilent = cmd->muzzleangle;
    C_BasePlayer* player = GetClosestPlayer(cmd, localplayer, bone, aimPoint, aimKeyDown, angSilent);
    if (player)
    {
        Vector eyePos = localplayer->GetEyePosition();

        static matrix3x4_t myBoneMatrix[NUM_BONE];
        localplayer->SetupBones(myBoneMatrix, NUM_BONE, BONE_USED_BY_HITBOX, 0);
        matrix3x4_t myBoneHead = myBoneMatrix[(int)Bone::HEAD];
        Vector vecMyBoneHead = Vector(myBoneHead[0][3], myBoneHead[1][3], myBoneHead[2][3]);
        if (Math::GetDistance(vecMyBoneHead, eyePos) > 12)
            eyePos = vecMyBoneHead;

        angle = Math::CalcAngle(eyePos, aimPoint);
        lastAimPoint = aimPoint;
    }

    Math::NormalizeAngles(angle);
    Math::ClampAngles(angle);

    if (aimKeyDown)
    {
        if (angle != cmd->viewangles)
        {
            QAngle dAngle = Math::DeltaAngles(cmd->viewangles, angle);
            MouseSim::sim(dAngle);
        }
    }

    if (angSilent != cmd->muzzleangle)
    {
        cmd->muzzleangle = angSilent;
    }
}
