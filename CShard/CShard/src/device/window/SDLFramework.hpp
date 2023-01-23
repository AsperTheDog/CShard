#pragma once

#include <SDL.h>
#include <unordered_set>
#include <vec2.hpp>
#include "../graphics/GUtils.hpp"

class SDLFramework
{
public:
	SDLFramework() = delete;

	static void init(GLibraries lib);
	static SDL_Window* getWindow();
	static std::vector<SDL_Event> getEvents(std::unordered_set<uint32_t>& subscribedTypes, bool isIDE);
	static void swapWindow();
	static void destroy();
	static glm::ivec2 getSize();
	static void showErrorMessage(std::string title, std::string text);
	static uint64_t getWindowTime();

	static float aspectRatio;

	inline static bool isMouseDragging = false;
	inline static bool shouldResetMouse = false;
	inline static bool leftClick = false;
private:
	static void manageIDEControls(SDL_Event& event);

	static GLibraries lib;
	static SDL_WindowFlags window_flags;
	static SDL_Window* window;
};

