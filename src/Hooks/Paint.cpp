#include "hooks.h"
#include "../logger.h"

extern StartDrawingFn StartDrawing;
extern FinishDrawingFn FinishDrawing;

std::vector<const char*> nodraw_materials = {
    "particles/ins_smoke_v2",
    "particles/ins_thick_smoke",
    "particles/ins_smoke_grenade",
    "particles/ins_loopsmoke",
    "particle/smoke1/smoke1"
    "particle/smoke1/smoke1",

    "particles/particle_glow_04_additive_noz"
    "particles/particle_glow_05"
    "particles/particle_glow_05_additive_noz_minmaxsize"
    "particles/particle_glow_05_additive_noz_minsize"
    "particles/particle_ring_wave_10_add_noz"
    "effects/softglow_translucent",
    "effects/yellowflare_noz",
    "particles/particle_flare_001"
    "particles/particle_flare_001_nodepth_noz_alpha",
    "particles/particle_flare_001_nodepth_noz_minsize",
    "particles/particle_flare_001_nodepth_noz_nearfade",
    "particles/particle_flare_001_nodepth_noz_ob",
    "particles/particle_flare_001_nodepth_noz",
    "particles/particle_flare_002_noz",
    "particles/particle_flare_006_noz",
    "particles/particle_flare_007b_noz",
    "particles/particle_flare_009_nodepth_noz_minmax",
    "particles/particle_glow_04_additive_noz",
    "particles/particle_glow_05_add_15ob_noz",
    "particles/particle_glow_05_additive_noz_minmaxsize",
    "particles/particle_glow_05_additive_noz_minsize",
    "particles/particle_meleeblur_noz",
    "particles/particle_modulate_01_noz_nodepth",
    "particles/particle_modulate_01_noz"
    "particles/particle_muzzleflash2_noz",
    "particles/particle_ring_wave_10_add_noz",
    "particles/particle_sphere_additive_noz_15ob",
    "sprites/glow1",
    "sprites/glow01",
    "sprites/glow02",
    "sprites/glow04",
    "sprites/glow04_noz"
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
                IMaterial* mat = material->FindMaterial(nodraw_materials[counter], TEXTURE_GROUP_OTHER);
                if (mat)
                    mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
            }
        }
        counter++;
        counter %= 1000;
    }
}
