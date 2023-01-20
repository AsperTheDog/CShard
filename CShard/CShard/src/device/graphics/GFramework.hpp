#pragma once
#include "GMesh.hpp"
#include "GTexture.hpp"
#include "GUtils.hpp"

#define DEFAULT_TEX_LOCATION ""

#define BASE_VERTEX_LOCATION "src/device/graphics/shaders/base.vert"
#define BASE_FRAGMENT_LOCATION "src/device/graphics/shaders/base.frag"

#define BACK_VERTEX_LOCATION "src/device/graphics/shaders/back.vert"
#define BACK_FRAGMENT_LOCATION "src/device/graphics/shaders/back.frag"

class GFramework
{
public:
	static void create(GLibraries lib);
	static GFramework* get();
	static void deleteInstance();

	virtual void init() = 0;
	virtual void initRender() = 0;
	virtual void endRender() = 0;
	virtual void resizeWindow() = 0;

	virtual void loadImGuiBackends() = 0;
	virtual void loadImGuiFrame() = 0;
	virtual void destroyImGui() = 0;
	virtual void renderImgui() = 0;

	virtual GMesh* createMesh(std::string filepath) = 0;
	virtual GTexture* createTexture(std::string filepath) = 0;

	virtual void setDefaultTexture() = 0;

protected:
	static std::string loadShaderSrc(const std::string& file);

	virtual ~GFramework() = default;
	inline static GFramework* instance = nullptr;
};
