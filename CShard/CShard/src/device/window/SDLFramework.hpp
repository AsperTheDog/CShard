#pragma once

#include <SDL.h>
#include <device/graphics/GFramework.hpp>

class SDLFramework
{
public:
	static void create(GLibraries lib);
	static SDLFramework* getInstance();

	void init();
	SDL_Window* getWindow();
	bool shouldWindowClose();

private:
	SDLFramework(GLibraries lib);

	GLibraries lib;
	SDL_WindowFlags window_flags{};
	SDL_Window* window = nullptr;
	
	static SDLFramework* instance;
};

