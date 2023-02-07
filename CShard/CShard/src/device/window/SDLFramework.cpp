#include "SDLFramework.hpp"

#include <stdexcept>
#include <backends/imgui_impl_sdl.h>
#include "../../ide/ImGuiManager.hpp"
#include "../graphics/GFramework.hpp"

SDL_WindowFlags SDLFramework::window_flags;
SDL_Window* SDLFramework::window;
float SDLFramework::aspectRatio;

void SDLFramework::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error("Error: " + std::string(SDL_GetError()));
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    window_flags = (SDL_WindowFlags)(window_flags | SDL_WINDOW_MAXIMIZED);
    SDLFramework::window = SDL_CreateWindow("CShard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
	aspectRatio = 1280 / 720.f;
}

SDL_Window* SDLFramework::getWindow()
{
    return SDLFramework::window;
}

glm::ivec2 SDLFramework::getSize()
{
    int width, height;
    SDL_GetWindowSize(SDLFramework::getWindow(), &width, &height);
    return {width, height};
}

void SDLFramework::showErrorMessage(const std::string& title, const std::string& text)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), text.c_str(), SDLFramework::getWindow());
}

uint64_t SDLFramework::getWindowTime()
{
    return SDL_GetTicks64();
}

void SDLFramework::manageIDEControls(SDL_Event& event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if (!isMouseDragging)
        {
			isMouseDragging = true;

			int x;
			int y;
			SDL_GetMouseState(&x, &y);
			ImGuiManager::lastMousePos.x = (float)x;
			ImGuiManager::lastMousePos.y = (float)y;
        }
	}
    if (!(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)))
	{
		isMouseDragging = false;
	}
    else
    {
        leftClick = true;
    }
    if (event.type == SDL_MOUSEMOTION)
    {
		ImGuiManager::mousePos.x = (float)event.motion.x;
		ImGuiManager::mousePos.y = (float)event.motion.y;
    }
    if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) 
        && (event.key.keysym.sym == SDLK_w 
            || event.key.keysym.sym == SDLK_a 
            || event.key.keysym.sym == SDLK_s
            || event.key.keysym.sym == SDLK_d
			|| event.key.keysym.sym == SDLK_SPACE
            || event.key.keysym.sym == SDLK_LSHIFT))
    {
        ImGuiManager::keyDowns.insert_or_assign(event.key.keysym.sym, event.type == SDL_KEYDOWN);
    }
    else if(event.type == SDL_KEYDOWN && event.key.keysym.mod & KMOD_CTRL)
    {
        if (event.key.keysym.sym == SDLK_c)
			ImGuiManager::copied = true;
        else if (event.key.keysym.sym == SDLK_v)
            ImGuiManager::pasted = true;
    }

}

std::vector<SDL_Event> SDLFramework::getEvents(std::unordered_set<uint32_t>& subscribedTypes, bool isIDE)
{
    std::vector<SDL_Event> events;

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if (isIDE) manageIDEControls(event);

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            int width, height;
            SDL_GetWindowSize(SDLFramework::getWindow(), &width, &height);
            aspectRatio = (float)width / (float)height;
	        GFramework::resizeWindow();
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
