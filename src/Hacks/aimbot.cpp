#include "aimbot.h"
#include "../interfaces.h"
#include "../Utils/mousesim.h"
#include <math.h>
#include "hacks.h"
#include "../settings.h"

#define NUM_BONE 128

float forceSelectFov = 1;

namespace Aimbot
{
    int targetIdx = -1;
    Vector lastAimPoint;
}


C_BasePlayer* GetClosestPlayer(CUserCmd* cmd, C_BasePlayer* localplayer, Bone& bestBone, Vector& aimPoint, bool locked, QAngle& angSilent, QAngle& angSilentHip)
{
    TeamID myteam = localplayer->GetTeam();
    Vector eyePos = localplayer->GetEyePosition();

    C_BasePlayer* closestEntity = NULL;
    float bestFov = Settings::Aimbot::fov;

    for (int i = 0; i <= engine->GetMaxClients(); i++)
    {
        C_BasePlayer* player = (C_BasePlayer*) entityList->GetClientEntity(i);
        if (!player
            || player == localplayer
            || player->GetTeam() == myteam
            || player->IsDormant()
            || !player->GetAlive())
        {
            continue;
        }

        if (locked
                && i != Aimbot::targetIdx
                && Math::GetDistance(Aimbot::lastAimPoint, player->GetEyePosition()) > Settings::Aimbot::cont_thresh*3*12)
        {
            continue;
        }

        Vector vecEyePos = player->GetEyePosition();
        QAngle angToEnemyEye = Math::CalcAngle(eyePos, vecEyePos);
        float fov = Math::GetFov(cmd->viewangles, angToEnemyEye);

        Bone targetBones[] = { Bone::SPINE_2, Bone::NECK, Bone::PELVIS, Bone::ELBOW_L, Bone::ELBOW_R, Bone::KNEE_L, Bone::KNEE_R };
        static matrix3x4_t BoneMatrix[NUM_BONE];
        player->SetupBones(BoneMatrix, NUM_BONE, BONE_USED_BY_HITBOX, 0);

        matrix3x4_t boneHead = BoneMatrix[(int)Bone::NECK];
        Vector vecBoneHead = Vector(boneHead[0][3], boneHead[1][3], boneHead[2][3])
                + Vector(boneHead[0][0], boneHead[1][0], boneHead[2][0]) * 5;
        if (Util::Ray(localplayer, player, i, eyePos, vecBoneHead))
        {
            QAngle angHead = Math::CalcAngle(eyePos, vecBoneHead);
            if (Math::GetFov(cmd->viewangles, angHead) < Settings::Aimbot::silent_fov)
            {
                angSilent = angHead;
            }

            if (Math::GetFov(cmd->viewangles, angHead) < Settings::Aimbot::silent_fov_hip &&
                    (locked || !localplayer->IsScoped()))
            {
                angSilentHip = angHead;
            }
        }

        if (fov > bestFov)
            continue;

        // find best bone for legit aim
        bool found = false;
        for (Bone b : targetBones)
        {
            matrix3x4_t currBone = BoneMatrix[(int)b];
            Vector vecBone = Vector(currBone[0][3], currBone[1][3], currBone[2][3]);

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

        matrix3x4_t currBone = BoneMatrix[(int)Bone::NECK];
        Vector headTop = Vector(currBone[0][3], currBone[1][3], currBone[2][3])
            + Vector(boneHead[0][0], boneHead[1][0], boneHead[2][0]) * 5;
        
        if (!found && (fov < forceSelectFov || Util::Ray(localplayer, player, i, eyePos, headTop)))
        {
            Aimbot::targetIdx = i;
            closestEntity = player;
            bestFov = fov;
            bestBone = Bone::NECK;
            aimPoint = headTop;
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
    if (!Settings::Aimbot::enable)
        return;

    MouseSim::update(cmd);

    bool aimKeyDown = inputSystem->IsButtonDown(Settings::Aimbot::key);

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

    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
    if (!activeWeapon || activeWeapon->IsGrenade())
        return;

    Bone bone = Bone::SPINE_3;
    Vector aimPoint;
    QAngle angSilent = cmd->viewangles;
    QAngle angSilentHip = cmd->muzzleangle;
    C_BasePlayer* player = GetClosestPlayer(cmd, localplayer, bone, aimPoint, aimKeyDown, angSilent, angSilentHip);
    if (player)
    {
        Vector eyePos = localplayer->GetEyePosition();

        static matrix3x4_t myBoneMatrix[NUM_BONE];
        localplayer->SetupBones(myBoneMatrix, NUM_BONE, BONE_USED_BY_HITBOX, 0);
        matrix3x4_t myBoneHead = myBoneMatrix[(int)Bone::NECK];
        Vector vecMyBoneHead = Vector(myBoneHead[0][3], myBoneHead[1][3], myBoneHead[2][3]);
        if (Math::GetDistance(vecMyBoneHead, eyePos) > 12)
            eyePos = vecMyBoneHead;

        angle = Math::CalcAngle(eyePos, aimPoint);
        lastAimPoint = aimPoint;
    }

    Math::NormalizeAngles(angle);
    Math::ClampAngles(angle);
    static QAngle lastAimPunch = *localplayer->GetAimPunchAngle();
    QAngle currAimPunch = *localplayer->GetAimPunchAngle();
    if (aimKeyDown)
    {

        if (activeWeapon->GetFiremode() != 0 && (cmd->buttons & IN_ATTACK))
        {
            float diffx = (currAimPunch.x - lastAimPunch.x) * Settings::Aimbot::recoilx;
            float diffy = (currAimPunch.y - lastAimPunch.y) * Settings::Aimbot::recoily;

            angle.x -= diffx;
            angle.y -= diffy;

            engine->SetViewAngles(cmd->viewangles);
        }

        QAngle dAngle = Math::DeltaAngles(cmd->viewangles, angle);
        MouseSim::sim(dAngle);

        if (angSilent != cmd->viewangles && localplayer->IsScoped())
        {
            cmd->viewangles = angSilent;
            cmd->muzzleangle = angSilent;
        }
    }

    lastAimPunch = currAimPunch;

    if (angSilentHip != cmd->muzzleangle)
    {
        cmd->muzzleangle = angSilentHip;
    }
}
