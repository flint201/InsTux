#pragma once

#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "../Utils/entity.h"
#include "../Utils/math.h"

namespace Aimbot
{
    extern int targetIdx;
    extern Vector lastAimPoint;
    void CreateMove(CUserCmd* cmd);
}
