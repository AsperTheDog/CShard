#pragma once
#include <glm.hpp>
#include <string>

#include "GUtils.hpp"

#define DEFAULT_TEX_LOCATION "pak/resources/tex/__noTex.png"

#define BASE_VERTEX_LOCATION "pak/shaders/base.vert"
#define BASE_FRAGMENT_LOCATION "pak/shaders/base.frag"

#define BACK_VERTEX_LOCATION "pak/shaders/back.vert"
#define BACK_FRAGMENT_LOCATION "pak/shaders/back.frag"

class BackGMesh;
class GTexture;
class GMesh;
class Model;
class Camera;

class GFramework
{
public:
	static void create(GLibraries lib);
	static GFramework* get();
	static void deleteInstance();
	
	virtual void initRender() = 0;
	virtual void endRender() = 0;
	virtual void resizeWindow(int width, int height) = 0;

	virtual void loadImGuiBackends() = 0;
	virtual void loadImGuiFrame() = 0;
	virtual void destroyImGui() = 0;
	virtual void renderImgui() = 0;

	virtual GMesh* createMesh(std::string filepath) = 0;
	virtual GTexture* createTexture(std::string filepath) = 0;

	virtual void setDefaultTexture() = 0;
	virtual void loadCamUniforms(Camera* camera) = 0;
	virtual void loadModelUniforms(Model* mod, glm::mat4& parent) = 0;
	virtual uint32_t getImGuiTexture() = 0;

	static GTexture* defaultTex;
	static BackGMesh* backgroundMesh;
	static GLibraries type;

	inline static glm::uvec2 imGuiSize{1920, 1080};
protected:
	inline static glm::uvec2 viewPortSize{1920, 1080};
	static std::string loadShaderSrc(const std::string& file);

	virtual ~GFramework() = default;
	inline static GFramework* instance = nullptr;
};
