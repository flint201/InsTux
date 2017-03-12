#include "fakelag.h"

static int ticks = 0;
int ticksMax = 16;

namespace FakeLag
{
    bool enable = false;
    int lagDuration = 0;
}

void FakeLag::Lag(int dur)
{
    lagDuration = dur;
}

void FakeLag::CreateMove(CUserCmd* cmd)
{
    static bool buttonPressed = false;
    if (inputSystem->IsButtonDown(Settings::FakeLag::key))
    {
        if (!buttonPressed)
            enable = !enable;

        buttonPressed = true;
    }
    else
    {
        buttonPressed = false;
    }


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
        CreateMove::sendPacket = ticks > Settings::FakeLag::value;
    }

    ticks++;
    if (lagDuration > 0)
        lagDuration--;
}
