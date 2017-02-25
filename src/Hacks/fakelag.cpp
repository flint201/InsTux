#include "fakelag.h"

static int ticks = 0;
int ticksMax = 16;
int lag_value = 8;

namespace FakeLag
{
    bool enable = true;
    int lagDuration = 0;
}

void FakeLag::Lag(int dur)
{
    lagDuration = dur;
}

void FakeLag::CreateMove(CUserCmd* cmd)
{
    if (!enable || lagDuration == 0)
    {
        ticks = 0;
        return;
    }

	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer || !localplayer->GetAlive())
		return;

	if (cmd->buttons & IN_ATTACK)
	{
		CreateMove::sendPacket = true;
		return;
	}

	if (ticks >= ticksMax)
	{
		CreateMove::sendPacket = true;
		ticks = 0;
	}
	else
	{
		CreateMove::sendPacket = ticks > lag_value;
	}

	ticks++;
    lagDuration--;
}
