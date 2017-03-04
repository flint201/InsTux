#include "noflash.h"
#include "hacks.h"
#include "../settings.h"

void NoFlash::Paint()
{
    C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!localplayer)
        return;

    float* flashMaxAlpha = localplayer->GetFlashMaxAlpha();
    *flashMaxAlpha = Settings::NoFlash::alpha;
}
