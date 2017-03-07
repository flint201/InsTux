#include "hooks.h"
#include "../logger.h"

extern StartDrawingFn StartDrawing;
extern FinishDrawingFn FinishDrawing;

std::vector<const char*> nodraw_materials = {
    "particles/ins_smoke_v2",
    "particles/ins_thick_smoke",
    "particles/ins_thin_smoke",
    "particles/ins_loopsmoke",
    "particles/ins_loopsmoke_thin",
    "particles/smoke_grenade",

    "particle/smoke1/smoke1",
    "particle/smoke_multi1/smoke_01"
};

void Hooks::Paint(void* thisptr, PaintMode_t mode)
{
    engineVGuiVMT->GetOriginalMethod<PaintFn>(15)(thisptr, mode);

    if (engine->IsTakingScreenshot())
        return;

    if (mode & PAINT_UIPANELS)
    {
        /*
        StartDrawing(surface);
        FinishDrawing(surface);
        */
        ESP::Paint();
        Radar::Paint();
        NoFlash::Paint();

        static unsigned counter = 0;
        if (counter < nodraw_materials.size())
        {
            C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
            if (localplayer)
            {
                IMaterial* mat = material->FindMaterial(nodraw_materials[counter], TEXTURE_GROUP_PARTICLE);
                if (mat)
                    mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
            }
        }
        counter++;
        counter %= 1000;
    }
}
