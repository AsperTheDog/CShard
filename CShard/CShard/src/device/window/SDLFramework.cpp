#include "SDLFramework.hpp"

#include <stdexcept>
#include "backends/imgui_impl_sdl.h"
#include "../graphics/GFramework.hpp"
#include "../../Engine.hpp"

GLibraries SDLFramework::lib;
SDL_WindowFlags SDLFramework::window_flags;
SDL_Window* SDLFramework::window;

void SDLFramework::init(GLibraries lib)
{
    SDLFramework::lib = lib;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error("Error: " + std::string(SDL_GetError()));
    }

    if (lib == OPENGL)
    {
        const char* glsl_version = "#version 450";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	    window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    }
    else
    {
        window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    }
    window_flags = (SDL_WindowFlags)(window_flags | SDL_WINDOW_MAXIMIZED);
    SDLFramework::window = SDL_CreateWindow("CShard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
}

SDL_Window* SDLFramework::getWindow()
{
    return SDLFramework::window;
}

std::vector<SDL_Event> SDLFramework::getEvents(std::unordered_set<uint32_t>& subscribedTypes)
{
    std::vector<SDL_Event> events;

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if (Engine::isIDE) ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
	        GFramework::get()->resizeWindow();
        }
        if (subscribedTypes.contains(event.type))
			events.push_back(event);
    }
    return events;
}

void SDLFramework::swapWindow()
{
    SDL_GL_SwapWindow(SDLFramework::window);
}

void SDLFramework::destroy()
{
    SDL_DestroyWindow(SDLFramework::window);
    SDL_Quit();
}
