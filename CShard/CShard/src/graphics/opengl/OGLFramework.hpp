#pragma once
#include "../GFramework.hpp"

#include <SDL.h>

#define GLMAYOR 4
#define GLMINOR 5

class OGLFramework final : public GFramework
{
public:
	void init() override;
	void loadImGuiBackends() override;
	void loadImGuiFrame() override;
	void destroyImGui() override;

private:
	SDL_WindowFlags window_flags{};
	SDL_Window* window = nullptr;
	SDL_GLContext gl_context = nullptr;
};