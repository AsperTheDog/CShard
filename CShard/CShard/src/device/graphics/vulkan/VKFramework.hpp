#pragma once
#include "../GFramework.hpp"

class VKFramework final : public GFramework
{
public:
	void init() override;
	void initRender() override;
	void endRender() override;
	void resizeWindow() override;

	void loadImGuiBackends() override;
	void loadImGuiFrame() override;
	void destroyImGui() override;
	void renderImgui() override;

	GMesh* createMesh(std::string filepath) override;
	GTexture* createTexture(std::string filepath) override;

	void setDefaultTexture() override;
};

