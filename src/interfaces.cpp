#include "interfaces.h"

#include <unistd.h>
#include <stdarg.h>

#include <vector>
#include <unordered_map>
#include <set>

#include "logger.h"

MsgFn _msgfn;

void Msg(const char* msg, ...) {
    if (msg == nullptr)
        return; //If no string was passed, or it was null then don't do anything

    char buffer[999];
    va_list list; //Normal varargs stuff http://stackoverflow.com/questions/10482960/varargs-to-printf-all-arguments
    va_start(list, msg);
    vsprintf(buffer, msg, list);
    va_end(list);
    _msgfn(buffer, list); //Calls the function, we got the address above.
}


ICvar* cvar = nullptr;

IBaseClientDLL* client = nullptr;
IClientEntityList* entityList = nullptr;
IClientMode* clientMode = nullptr;

IEngineClient* engine = nullptr;
IEngineTrace* trace = nullptr;
IVModelRender* modelRender = nullptr;
IMaterialSystem* material = nullptr;
IVModelInfo* modelInfo = nullptr;

ISurface* surface = nullptr;
IEngineVGui* engineVGui = nullptr;

IVDebugOverlay* debugOverlay = nullptr;
/*
IVPanel* panel = nullptr;
IInputSystem* inputSystem = nullptr;
IInputInternal* inputInternal = nullptr;
CGlobalVars* globalVars = nullptr;
CEffects* effects = nullptr;
IGameEventManager2* gameEvents = nullptr;
IPhysicsSurfaceProps* physics = nullptr;
CViewRender* viewRender = nullptr;
IPrediction* prediction = nullptr;
IGameMovement* gameMovement = nullptr;
IMoveHelper* moveHelper = nullptr;
ILauncherMgr* launcherMgr = nullptr;
CGlowObjectManager* glowManager = nullptr;
C_CSPlayerResource** csPlayerResource = nullptr;
C_CSGameRules** csGameRules = nullptr;
IEngineSound* sound = nullptr;
ILocalize* localize = nullptr;
ICommandLine* commandline = nullptr;
CInput* input = nullptr;
CGameServer* sv = nullptr;
*/
char cwd[1024];

void checknull(void* ptr, std::string varname)
{
    if (ptr == nullptr)
        Log << "- nullptr " << varname << std::endl;
    else
        //std::ofstream(std::string(cwd) + "/instux.log", std::ofstream::app) << "+ " << varname << " = " << (unsigned) ptr << std::endl;
        Log << "+ " << varname << " = " << hex((unsigned)ptr) << std::endl;
}

void Interfaces::FindInterfaces()
{
	getcwd(cwd, sizeof(cwd));

	//cvar = GetInterface<ICvar>("./bin/materialsystem.so", "VEngineCvar");

    void* hLibtier0 = dlopen("./bin/libtier0.so", 1);
    checknull(hLibtier0, "libtier0.so");
    
    _msgfn = (MsgFn)dlsym(hLibtier0, "Msg");
    checknull((void*)_msgfn, "_msgfn");

    client = GetInterface<IBaseClientDLL>("./insurgency/bin/client.so", "VClient");
    checknull(client, "client");

	entityList = GetInterface<IClientEntityList>("./insurgency/bin/client.so", "VClientEntityList");
    checknull(entityList, "entityList");


	engine = GetInterface<IEngineClient>("./bin/engine.so", "VEngineClient");
    checknull(engine, "engine");

	modelRender = GetInterface<IVModelRender>("./bin/engine.so", "VEngineModel");
    checknull(modelRender, "modelRender");

	modelInfo = GetInterface<IVModelInfo>("./bin/engine.so", "VModelInfoClient");
    checknull(modelInfo, "modelInfo");

	trace = GetInterface<IEngineTrace>("./bin/engine.so", "EngineTraceClient");
    checknull(trace, "trace");

	material = GetInterface<IMaterialSystem>("./bin/materialsystem.so", "VMaterialSystem");
    checknull(material, "material");


	surface = GetInterface<ISurface>("./bin/vguimatsurface.so", "VGUI_Surface");
    checknull(surface, "surface");

	engineVGui = GetInterface<IEngineVGui>("./bin/engine.so", "VEngineVGui");
    checknull(engineVGui, "engineVGui");

	debugOverlay = GetInterface<IVDebugOverlay>("./bin/engine.so", "VDebugOverlay");
    checknull(debugOverlay, "debugOverlay");
    /*
	panel = GetInterface<IVPanel>("./bin/linux64/vgui2_client.so", "VGUI_Panel");
	inputSystem = GetInterface<IInputSystem>("./bin/linux64/inputsystem_client.so", "InputSystemVersion");
	inputInternal = GetInterface<IInputInternal>("./bin/linux64/vgui2_client.so", "VGUI_InputInternal");
	effects = GetInterface<CEffects>("./bin/linux64/engine_client.so", "VEngineEffects");
	gameEvents = GetInterface<IGameEventManager2>("./bin/linux64/engine_client.so", "GAMEEVENTSMANAGER002", true);
	physics = GetInterface<IPhysicsSurfaceProps>("./bin/linux64/vphysics_client.so", "VPhysicsSurfaceProps");
	prediction = GetInterface<IPrediction>("./csgo/bin/linux64/client_client.so", "VClientPrediction");
	gameMovement = GetInterface<IGameMovement>("./csgo/bin/linux64/client_client.so", "GameMovement");
	sound = GetInterface<IEngineSound>("./bin/linux64/engine_client.so", "IEngineSoundClient");
	localize = GetInterface<ILocalize>("./bin/linux64/localize_client.so", "Localize_");
	commandline = GetSymbolAddress<CommandLineFn>("./bin/linux64/libtier0_client.so", "CommandLine")();
    */
}


void Interfaces::DumpInterfaces()
{
	std::stringstream ss;
	char cwd[1024];

	std::vector<const char*> modules;

	dl_iterate_phdr([](struct dl_phdr_info* info, size_t size, void* data) {
		reinterpret_cast<std::vector<const char*>*>(data)->push_back(info->dlpi_name);
		return 0;
	}, &modules);

	for (auto module : modules)
	{
		void* library = dlopen(module, RTLD_NOLOAD | RTLD_NOW);

		if (!library)
			continue;

		void* interfaces_sym = dlsym(library, "s_pInterfaceRegs");

		if (!interfaces_sym)
		{
			dlclose(library);
			continue;
		}

		dlclose(library);

		InterfaceReg* interfaces = *reinterpret_cast<InterfaceReg**>(interfaces_sym);
		InterfaceReg* cur_interface;

		std::set<const char*> interface_name;

		for (cur_interface = interfaces; cur_interface; cur_interface = cur_interface->m_pNext)
			interface_name.insert(cur_interface->m_pName);

		if (interface_name.empty())
			continue;

		ss << std::string(module) << "\n";
		for (auto interface : interface_name)
			ss << "    " << interface << "\n";

		ss << '\n';
	}

	getcwd(cwd, sizeof(cwd));
	std::string interfacesPath = std::string(cwd) + "/interfaces.txt";
	std::ofstream(interfacesPath) << ss.str();
}
