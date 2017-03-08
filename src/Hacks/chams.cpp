#include "chams.h"
#include "../logger.h"
#include <unistd.h>

const MaterialVarFlags_t hidden_MaterialVarFlags = (MaterialVarFlags_t) (
        MATERIAL_VAR_IGNOREZ
        );

const MaterialVarFlags_t brightFlags = (MaterialVarFlags_t) (
        //MATERIAL_VAR_ADDITIVE |
        //MATERIAL_VAR_VERTEXCOLOR |
        MATERIAL_VAR_HALFLAMBERT |
        MATERIAL_VAR_NOCULL |
        MATERIAL_VAR_NOFOG
        );

IMaterial* materialWhite;

bool Chams::DrawModelExecute(void* thisptr, void* context, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld)
{
    if (Settings::Cham::only_on_key_down && !inputSystem->IsButtonDown(Settings::Cham::key))
        return false;

    if (!pInfo.pModel)
        return false;

    static int counter = 0;
    static TeamID myteam;
    if (counter % 1000 == 0) {
        counter = 0;
        C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
        myteam = localplayer->GetTeam();
    }
    counter++;

    std::string modelName = modelInfo->GetModelName(pInfo.pModel);
    if (modelName.find("/characters") == std::string::npos)
        return false;
    
    static bool materialInitialized = false;
    if (!materialInitialized) {
        ///*
        modelInfo->GetModelMaterials(pInfo.pModel, 1, &materialWhite);
        std::string materialName = materialWhite->GetName();
        if (materialName.find("models/player/attachments/ins_attachments_color") != std::string::npos)
            materialInitialized = true;
        //*/

        //materialWhite = material->FindMaterial("mat_white", TEXTURE_GROUP_MODEL);
        //materialInitialized = true;
        //Log << " ** FindMaterial returned " << materialWhite << std::endl;
    }

    C_BasePlayer* entity = (C_BasePlayer*) entityList->GetClientEntity(pInfo.entity_index);
    
    if (!entity || entity->IsDormant() || !entity->GetAlive() || entity->GetTeam() == myteam)
    {
        materialWhite->ColorModulate(0.1, 0.1, 0.1);
        modelRender->ForcedMaterialOverride(materialWhite);
        return false;
    }

    // draw hidden
    materialWhite->SetMaterialVarFlag(hidden_MaterialVarFlags, true);
    //materialWhite->SetMaterialVarFlag(brightFlags, true);
    //Color colorHid = Settings::Cham::color_hidden;
    //materialWhite->ColorModulate((float)colorHid.r/255, (float)colorHid.g/255, (float)colorHid.b/255);
    materialWhite->ColorModulate(255, 0, 0);
    modelRender->ForcedMaterialOverride(materialWhite);
    modelRenderVMT->GetOriginalMethod<DrawModelExecuteFn>(DRAW_MODEL_EXECUTE_IDX)(thisptr, context, state, pInfo, pCustomBoneToWorld);

    // draw visible
    materialWhite->SetMaterialVarFlag(hidden_MaterialVarFlags, false);
    //Color colorVis = Settings::Cham::color_visible;
    //materialWhite->ColorModulate((float)colorVis.r/255, (float)colorVis.g/255, (float)colorVis.b/255);
    materialWhite->ColorModulate(0, 255, 0);
    modelRender->ForcedMaterialOverride(materialWhite);
    modelRenderVMT->GetOriginalMethod<DrawModelExecuteFn>(DRAW_MODEL_EXECUTE_IDX)(thisptr, context, state, pInfo, pCustomBoneToWorld);

    return true;
}
