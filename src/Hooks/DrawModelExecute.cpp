#include "hooks.h"
#include <unistd.h>


void Hooks::DrawModelExecute(void* thisptr, void* context, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld)
{
    if (engine->IsTakingScreenshot())
    {
        modelRender->ForcedMaterialOverride(NULL);
        modelRenderVMT->GetOriginalMethod<DrawModelExecuteFn>(DRAW_MODEL_EXECUTE_IDX)(thisptr, context, state, pInfo, pCustomBoneToWorld);
        return;
    }
    else if (!Chams::DrawModelExecute(thisptr, context, state, pInfo, pCustomBoneToWorld))
    {
        modelRenderVMT->GetOriginalMethod<DrawModelExecuteFn>(DRAW_MODEL_EXECUTE_IDX)(thisptr, context, state, pInfo, pCustomBoneToWorld);
    }

    modelRender->ForcedMaterialOverride(NULL);
}
