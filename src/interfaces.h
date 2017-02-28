#pragma once

#include <unordered_map>
#include <sys/mman.h>
#include <link.h>
#include <fstream>
#include "SDK/SDK.h"
#include "Utils/patternfinder.h"
#include "Utils/vmt.h"
#include "Utils/util.h"

typedef void(*MsgFn)(const char* msg, va_list);
void Msg(const char* msg, ...);

extern ICvar* cvar;

extern IBaseClientDLL* client;
extern IClientEntityList* entityList;
extern IClientMode* clientMode;
extern IEngineClient* engine;
extern IEngineTrace* trace;
extern IVModelRender* modelRender;
extern IMaterialSystem* material;
extern IVModelInfo* modelInfo;

extern ISurface* surface;
extern IEngineVGui* engineVGui;

extern VMT* materialVMT;
extern VMT* modelRenderVMT;
extern VMT* clientModeVMT;
extern VMT* clientVMT;

extern VMT* surfaceVMT;
extern VMT* engineVGuiVMT;

extern InitKeyValuesFn InitKeyValues;
extern LoadFromBufferFn LoadFromBuffer;

extern IVDebugOverlay* debugOverlay;
extern IInputSystem* inputSystem;

extern bool* bSendPacket;
/*
extern IVPanel* panel;
extern IInputInternal* inputInternal;
extern CGlobalVars* globalVars;
extern CEffects* effects;
extern IGameEventManager2* gameEvents;
extern IPhysicsSurfaceProps* physics;
extern CViewRender* viewRender;
extern IPrediction* prediction;
extern IGameMovement* gameMovement;
extern IMoveHelper* moveHelper;
extern CGlowObjectManager* glowManager;
extern ILauncherMgr* launcherMgr;
extern C_CSPlayerResource** csPlayerResource;
extern C_CSGameRules** csGameRules;
extern IEngineSound* sound;
extern ILocalize* localize;
extern ICommandLine* commandline;
extern CInput* input;
extern CGameServer* sv;

extern VMT* panelVMT;
extern VMT* gameEventsVMT;
extern VMT* viewRenderVMT;
extern VMT* inputInternalVMT;
extern VMT* launcherMgrVMT;
extern VMT* soundVMT;

extern LineGoesThroughSmokeFn LineGoesThroughSmoke;

extern int* nPredictionRandomSeed;
extern CMoveData* g_MoveData;
extern uint8_t* CrosshairWeaponTypeCheck;
extern uint8_t* CamThinkSvCheatsCheck;

extern uintptr_t oSwapWindow;
extern uintptr_t* swapWindowJumpAddress;

extern uintptr_t oPollEvent;
extern uintptr_t* polleventJumpAddress;
*/

namespace Interfaces
{
	void FindInterfaces();
	void DumpInterfaces();
}
