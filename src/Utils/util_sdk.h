#pragma once

#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "util.h"

namespace Util
{
    /*
	char GetButtonString(ButtonCode_t key);
	std::string GetButtonName(ButtonCode_t buttonCode);
	ButtonCode_t GetButtonCode(std::string buttonName);
    */

	IMaterial* CreateMaterial(std::string type, std::string texture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert);
    bool Ray(C_BasePlayer* localplayer, Vector vecStart, Vector vecEnd);
}
