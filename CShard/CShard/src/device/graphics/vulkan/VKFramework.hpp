#pragma once
#include "../GFramework.hpp"

class VKFramework final : public GFramework
{
public:
	VKFramework();

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
	void loadCamUniforms(Camera& camera) override;
	void loadModelUniforms(Model& mod, bool material) override;
	uint32_t getImGuiTexture() override;
	GCubeTexture* createCubeTexture(uint32_t width, uint32_t height) override;
	void loadLightUniforms(Light& light, PhysicalData&) override;
	void setPostUniforms() override;
	void prepareShader(ShaderType type) override;
	GShadowMap* createShadowMap(uint32_t size) override;
};

