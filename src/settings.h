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
        extern float k;
    }

    void LoadDefaultsOrSave(std::string path);
    void LoadConfig();
}
