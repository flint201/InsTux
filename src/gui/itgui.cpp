#include "itgui.h"
#include <dlfcn.h>

bool GUI::visible = true;

uintptr_t* swapwindow_ptr; // Pointer to 'SDL_GL_SwapWindow' in the jump table.
uintptr_t swapwindow_original; // Address of the original 'SDL_GL_SwapWindow'.

void SwapWindowHook(SDL_Window* window)
{
    // Get the original 'SDL_GL_SwapWindow' symbol from 'libSDL2-2.0.so.0'.
    static void (*oSDL_GL_SwapWindow) (SDL_Window*) = reinterpret_cast<void(*)(SDL_Window*)>(swapwindow_original);
    
    // Store OpenGL contexts.
    static SDL_GLContext original_context = SDL_GL_GetCurrentContext();
    static SDL_GLContext user_context = NULL;
    
    // Perform first-time initialization.
    if (!user_context) {
        // Create a new context for our rendering.
        user_context = SDL_GL_CreateContext(window);
        ImGui_ImplSdl_Init(window);
    }

    // Switch to our context.
    SDL_GL_MakeCurrent(window, user_context);

    // Perform UI rendering.
    ImGui_ImplSdl_NewFrame(window);
    ImGui::Text("InsTux!");
    ImGui::Render();

    // Swap back to the game context.
    SDL_GL_MakeCurrent(window, original_context);

    // Call the original function.
    oSDL_GL_SwapWindow(window);
}

// Credit @aixxe
void GUI::Init()
{
    Dl_info sdl_libinfo = {}; // Get the symbol address of 'SDL_GL_SwapWindow'.
    dladdr(dlsym(RTLD_NEXT, "SDL_GL_SwapWindow"), &sdl_libinfo);

    // Get the address of 'SDL_GL_SwapWindow' in the jump table.
    swapwindow_ptr = reinterpret_cast<uintptr_t*>(uintptr_t(sdl_libinfo.dli_fbase) + 0x104A88);
    
    // Backup the original address.
    swapwindow_original = *swapwindow_ptr;

    // Write the address to our replacement function.
    *swapwindow_ptr = reinterpret_cast<uintptr_t>(&SwapWindowHook);
}

void GUI::DeInit()
{
    *swapwindow_ptr = swapwindow_original; // Restore the original address.
}
