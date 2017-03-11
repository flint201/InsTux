#pragma once

#include <SDL2/SDL.h>
#include "../imgui/imgui.h"
#include "imgui_impl_sdl.h"
#include "../settings.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

namespace GUI
{
    extern bool visible;

    void Init();
    void DeInit();
}
