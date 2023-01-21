#pragma once
#include "../GFramework.hpp"

class VKFramework final : public GFramework
{
public:
	VKFramework();

	void initRender() override;
	void endRender() override;
	void resizeWindow(int width, int height) override;

	void loadImGuiBackends() override;
	void loadImGuiFrame() override;
	void destroyImGui() override;
	void renderImgui() override;

	GMesh* createMesh(std::string filepath) override;
	GTexture* createTexture(std::string filepath) override;

	void setDefaultTexture() override;
	void loadCamUniforms(Camera* camera) override;
	void loadModelUniforms(Model* mod, glm::mat4& parent) override;
	uint32_t getImGuiTexture() override;
};

