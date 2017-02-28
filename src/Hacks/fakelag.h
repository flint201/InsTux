#pragma once

#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "../Hooks/hooks.h"
#include "../Utils/util.h"

namespace FakeLag {
    extern bool enable;
    extern int lagDuration;
    void Lag(int dur);
    void CreateMove(CUserCmd* cmd);
};
