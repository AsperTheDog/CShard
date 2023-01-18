#pragma once
#include "../GFramework.hpp"

class VKFramework final : public GFramework
{
public:
	void init() override;
	void loadImGuiBackends() override;
	void loadImGuiFrame() override;
	void destroyImGui() override;
	void renderImgui() override;
	void render() override;
	void resizeWindow() override;
};

