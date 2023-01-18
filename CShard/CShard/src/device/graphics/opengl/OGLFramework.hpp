#pragma once
#include "../GFramework.hpp"
#include "../../window/SDLFramework.hpp"

#define GLMAYOR 4
#define GLMINOR 5

class OGLFramework final : public GFramework
{
public:
	void init() override;
	void loadImGuiBackends() override;
	void loadImGuiFrame() override;
	void destroyImGui() override;
	void renderImgui() override;
	void render() override;
	void resizeWindow() override;

private:
	SDL_GLContext gl_context = nullptr;
};