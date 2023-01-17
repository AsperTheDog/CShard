#pragma once
#include "device/graphics/GFramework.hpp"
#include "device/window/SDLFramework.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL_opengl.h>

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
	SDL_GLContext gl_context = nullptr;
};