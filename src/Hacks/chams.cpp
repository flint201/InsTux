#include "chams.h"
#include "../logger.h"
#include <unistd.h>

const MaterialVarFlags_t hidden_MaterialVarFlags = (MaterialVarFlags_t) (
        MATERIAL_VAR_IGNOREZ
        );

const MaterialVarFlags_t brightFlags = (MaterialVarFlags_t) (
        MATERIAL_VAR_HALFLAMBERT |
        MATERIAL_VAR_NOFOG
        );

IMaterial* materialHidden;

bool Chams::DrawModelExecute(void* thisptr, void* context, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld)
{
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
        modelInfo->GetModelMaterials(pInfo.pModel, 1, &materialHidden);
        Log << " -- got material name:" << materialHidden->GetName() << " texture group:" << materialHidden->GetTextureGroupName() << std::endl;
        std::string materialName = materialHidden->GetName();
        if (materialName.find("models/player/attachments/ins_attachments_color") != std::string::npos)
            materialInitialized = true;

        //materialHidden = material->FindMaterial("models/player/ins_heavy/ins_heavy_color", "Model textures");
        //Log << " ** FindMaterial returned " << materialHidden << std::endl;
    }

    C_BasePlayer* entity = (C_BasePlayer*) entityList->GetClientEntity(pInfo.entity_index);
    
    if (!entity || entity->GetDormant() || !entity->GetAlive() || entity->GetTeam() == myteam)
    {
        materialHidden->ColorModulate(0.2, 0.2, 0.2);
        modelRender->ForcedMaterialOverride(materialHidden);
        return false;
    }

    materialHidden->SetMaterialVarFlag(hidden_MaterialVarFlags, true);
    materialHidden->SetMaterialVarFlag(brightFlags, true);
    materialHidden->ColorModulate(255, 0, 0);
    modelRender->ForcedMaterialOverride(materialHidden);
    modelRenderVMT->GetOriginalMethod<DrawModelExecuteFn>(DRAW_MODEL_EXECUTE_IDX)(thisptr, context, state, pInfo, pCustomBoneToWorld);

    materialHidden->SetMaterialVarFlag(hidden_MaterialVarFlags, false);
    materialHidden->ColorModulate(0, 255, 0);
    modelRender->ForcedMaterialOverride(materialHidden);
    modelRenderVMT->GetOriginalMethod<DrawModelExecuteFn>(DRAW_MODEL_EXECUTE_IDX)(thisptr, context, state, pInfo, pCustomBoneToWorld);

    return true;
}
