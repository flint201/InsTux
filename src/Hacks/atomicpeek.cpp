#include "atomicpeek.h"
#include <math.h>
#include "hacks.h"
#include <vector>

float dt = 2;

void AtomicPeek::CreateMove(CUserCmd* cmd)
{
	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

	if (!localplayer || !localplayer->GetAlive())
    {
		return;
    }

    Vector displacement = localplayer->GetVelocity() * dt;


    static matrix3x4_t BoneMatrix[128];
    localplayer->SetupBones(BoneMatrix, 128, BONE_USED_BY_HITBOX, 0);

    Bone targetBones[] = { Bone::HEAD, Bone::ELBOW_L, Bone::ELBOW_R };
    std::vector<Vector> myBonePos;

    for (Bone b : targetBones)
    {
        matrix3x4_t currBone = BoneMatrix[(int)b];
        Vector vecBone = Vector(currBone[0][3], currBone[1][3], currBone[2][3]);
        vecBone = vecBone + displacement;
        myBonePos.push_back(vecBone);
    }

	for (int i = 1; i < engine->GetMaxClients(); i++)
	{
		C_BasePlayer* player = (C_BasePlayer*) entityList->GetClientEntity(i);
		if (!player
			|| player == localplayer
            || player->GetTeam() == localplayer->GetTeam()
			|| player->GetDormant()
			|| !player->GetAlive())
        {
			continue;
        }

        Vector playerEyePos = player->GetEyePosition();
        for (Vector bonePos : myBonePos)
        {
            if (Util::Ray(player, playerEyePos, bonePos))
            {
                FakeLag::Lag(32);
                return;
            }
        }
    }
}
