#pragma once
#include <glm.hpp>
#include <string>
#include <ext/matrix_clip_space.hpp>

#include "GShadowMap.hpp"
#include "GUtils.hpp"
#include "PostEffects.hpp"

#include "../../elements/components/Camera.hpp"

struct PhysicalData;
class GCubeTexture;
class Light;
class BackGMesh;
class GTexture;
class GMesh;
class Model;

enum ShaderType
{
	SHADER_BACKGROUND,
	SHADER_BASE,
	SHADER_POST,
	SHADER_SHADOW
};

class GFramework
{
public:
	static void create(GLibraries lib);
	static GFramework* get();
	static void deleteInstance();
	
	virtual void initRender() = 0;
	virtual void endRender() = 0;
	virtual void resizeWindow() = 0;

	virtual void loadImGuiBackends() = 0;
	virtual void loadImGuiFrame() = 0;
	virtual void destroyImGui() = 0;
	virtual void renderImgui() = 0;

	virtual GMesh* createMesh(std::string filepath) = 0;
	virtual GTexture* createTexture(std::string filepath) = 0;
	virtual GCubeTexture* createCubeTexture(uint32_t width, uint32_t height) = 0;

	virtual void setDefaultTexture() = 0;
	virtual void loadCamUniforms(Camera& camera) = 0;
	virtual void loadModelUniforms(Model& mod, bool material) = 0;
	virtual void loadLightUniforms(Light& light, PhysicalData&) = 0;
	virtual void setPostUniforms() = 0;
	virtual uint32_t getImGuiTexture() = 0;

	virtual void prepareShader(ShaderType type) = 0;
	virtual GShadowMap* createShadowMap(uint32_t size) = 0;

	static GTexture* defaultTex;
	static BackGMesh* fullQuadMesh;
	static GLibraries type;

	inline static glm::uvec2 imGuiSize{1920, 1080};
	inline static uint8_t lightSourceCount = 0;
	inline static Light* lights[10];

	inline static float postMult = 1.f;
	inline static FilmGrain filmGrain{};
	inline static bool postEffectsActive = false;
	inline static uint32_t activeShader = 0;

	inline static Camera shadowMapCam{};
protected:
	inline static glm::uvec2 viewPortSize{1920, 1080};
	static std::string loadShaderSrc(const std::string& file);

	virtual ~GFramework() = default;

	inline static GFramework* instance = nullptr;
};
