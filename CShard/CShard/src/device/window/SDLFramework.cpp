#include "SDLFramework.hpp"
#include <stdexcept>

void SDLFramework::create(GLibraries lib)
{
    instance = new SDLFramework(lib);
}

SDLFramework* SDLFramework::getInstance()
{
    return nullptr;
}

void SDLFramework::init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
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
    window = SDL_CreateWindow("CShard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
}

SDL_Window* SDLFramework::getWindow()
{
    return window;
}

bool SDLFramework::shouldWindowClose()
{
    return false;
}

SDLFramework::SDLFramework(GLibraries lib)
{
    this->lib = lib;
}
