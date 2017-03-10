#include "interfaces.h"
#include "hooker.h"
#include "./Hooks/hooks.h"
#include "logger.h"
#include "Utils/netvarmanager.h"
#include "Utils/mousesim.h"
#include "fonts.h"
#include "settings.h"

void CreateMaterialFile()
{
    char dir_mat[1024];
    getcwd(dir_mat, sizeof(dir_mat));
    strcat(dir_mat, "/insurgency/materials");

	struct stat info;
    if (!(stat(dir_mat, &info) == 0 && S_ISDIR(info.st_mode)))
		mkdir(dir_mat, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    FILE* fp = fopen("insurgency/materials/mat_white.vmt", "w+");
    if (fp)
    {
        const char* mat_str = "\"UnLitGeneric\"\n{\n$baseTexture \"vgui/white\"\n}";
        fwrite(mat_str, 1, strlen(mat_str), fp);
        fclose(fp);
    }
}

// Called on library load
int __attribute__((constructor)) instux_init()
{
    InitLogger();

    //CreateMaterialFile();

    Interfaces::FindInterfaces();
    Interfaces::DumpInterfaces();

    Msg("++++ InsTux starting... \n");

    Hooker::FindIClientMode();
    Hooker::FindSendPacket();

    Hooker::InitializeVMHooks();

    modelRenderVMT->HookVM((void*) Hooks::DrawModelExecute, DRAW_MODEL_EXECUTE_IDX);
    modelRenderVMT->ApplyVMT();

    clientModeVMT->HookVM((void*) Hooks::CreateMove, CREATE_MOVE_IDX);
    clientModeVMT->ApplyVMT();

    engineVGuiVMT->HookVM((void*) Hooks::Paint, 15);
    engineVGuiVMT->ApplyVMT();

    NetVarManager::DumpNetvars();
    Offsets::GetOffsets();
    Fonts::SetupFonts();

    MouseSim::mouseInit();

    Settings::LoadConfig();
    
    Msg("++++ InsTux loading complete! ++++\n");
    return 0;
}

// Called on library unload
void __attribute__((destructor)) instux_shutdown()
{
    MouseSim::mouseDestroy();

    modelRenderVMT->ReleaseVMT();
    clientModeVMT->ReleaseVMT();
    engineVGuiVMT->ReleaseVMT();
}
