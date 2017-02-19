#include "hooks.h"

QAngle CreateMove::lastTickViewAngles = QAngle(0, 0, 0);

bool Hooks::CreateMove(void* thisptr, float flInputSampleTime, CUserCmd* cmd)
{
	clientModeVMT->GetOriginalMethod<CreateMoveFn>(CREATE_MOVE_IDX)(thisptr, flInputSampleTime, cmd);

	if (cmd && cmd->command_number)
	{
		Aimbot::CreateMove(cmd);
	}

	return false;
}
