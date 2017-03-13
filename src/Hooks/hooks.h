#pragma once

#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "../Hacks/hacks.h"
#include "../hooker.h"

#define DRAW_MODEL_EXECUTE_IDX 19
#define CREATE_MOVE_IDX 25

typedef void (*DrawModelExecuteFn) (void*, void*, void*, const ModelRenderInfo_t&, matrix3x4_t*);
typedef bool (*CreateMoveFn) (void*, float, CUserCmd*);

typedef void (*PaintFn) (void*, PaintMode_t);

namespace Hooks
{
    void DrawModelExecute(void* thisptr, void* context, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld);
    bool CreateMove(void* thisptr, float flInputSampleTime, CUserCmd* cmd);

    void Paint(void* thisptr, PaintMode_t mode);
}

namespace CreateMove
{
        extern bool sendPacket;
        extern QAngle lastTickViewAngles;
        extern QAngle muzzleangle;
        extern Vector muzzlepos;
}
