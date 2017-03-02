#pragma once

#include <math.h>
#include "../SDK/SDK.h"
#include "../Hooks/hooks.h"
#include "../Utils/draw.h"
#include "../interfaces.h"
#include "../Utils/pstring.h"
#include "../Utils/entity.h"
//#include "../settings.h"
namespace ESP
{
    extern std::string dbgMsg;
    void Paint();
    void DrawSkeleton(C_BasePlayer* player);
}
