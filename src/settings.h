#pragma once

#include <unordered_map>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "json/json.h"
#include "SDK/SDK.h"
#include "fonts.h"
#include "Utils/draw.h"
#include "Utils/util.h"
#include "Utils/util_sdk.h"
#include "config.h"

namespace Settings
{
    namespace Aimbot
    {
        extern bool enable;
        extern ButtonCode_t key;
        extern float sensitivity;
        extern bool limit_aim_speed;
        extern float tu;
        extern float ku;
        extern float fov;
        extern float cont_thresh;

        extern float silent_fov;
        extern float silent_fov_hip;

        extern float recoilx;
        extern float recoily;
    }

    namespace Cham
    {
        extern bool only_on_key_down;
        extern ButtonCode_t key;
        extern Color color_visible;
        extern Color color_hidden;
    }

    namespace ESP
    {
        extern bool enable;
        extern ButtonCode_t key;
        extern bool show_bone;
        extern bool show_name;
        extern bool draw_behind;
    }

    namespace Radar
    {
        extern bool enable;
        extern int size;
        extern float range;
        extern float dot_radius;
        extern Color color_friendly;
        extern Color color_hostile;
        extern bool draw_level;
    }

    namespace NoFlash
    {
        extern float alpha;
    }

    namespace NoSmoke
    {
        extern bool enable;
    }

    namespace FakeLag
    {
        extern ButtonCode_t key;
        extern bool automatic;
        extern int value;
    }

    namespace GUI
    {
        extern Color color_main;
        extern Color color_main2;
        extern Color color_hl;
    }

    void SaveToFile(std::string path);
    void LoadConfig();
}
