#include "hooks.h"

QAngle CreateMove::lastTickViewAngles = QAngle(0, 0, 0);
bool CreateMove::sendPacket = true;

QAngle CreateMove::muzzleangle;
Vector CreateMove::muzzlepos;

bool Hooks::CreateMove(void* thisptr, float flInputSampleTime, CUserCmd* cmd)
{
    clientModeVMT->GetOriginalMethod<CreateMoveFn>(CREATE_MOVE_IDX)(thisptr, flInputSampleTime, cmd);

    if (cmd && cmd->command_number)
    {
        *bSendPacket = CreateMove::sendPacket;
        CreateMove::sendPacket = true;

        Aimbot::CreateMove(cmd);
        //AtomicPeek::CreateMove(cmd);
        //FakeLag::CreateMove(cmd);
        
        if (CreateMove::sendPacket)
            CreateMove::lastTickViewAngles = cmd->viewangles;
        
        CreateMove::muzzleangle = cmd->muzzleangle;
        CreateMove::muzzlepos = cmd->muzzlepos;
    }

    return false;
}
