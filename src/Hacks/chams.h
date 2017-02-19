#pragma once

#include <sstream>
#include <fstream>
#include <zconf.h>
#include <pwd.h>
#include "../SDK/SDK.h"
#include "../Hooks/hooks.h"
#include "../interfaces.h"
#include "../Utils/entity.h"
#include "../Utils/util_sdk.h"

namespace Chams
{
	bool DrawModelExecute(void* thisptr, void* context, void *state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld);
}
