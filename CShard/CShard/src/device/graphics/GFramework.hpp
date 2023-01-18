#pragma once
#include "GUtils.hpp"

class GFramework
{
public:
	static void create(GLibraries lib);
	static GFramework* get();
	static void deleteInstance();

	virtual void init() = 0;
	virtual void render() = 0;
	virtual void resizeWindow() = 0;

	virtual void loadImGuiBackends() = 0;
	virtual void loadImGuiFrame() = 0;
	virtual void destroyImGui() = 0;
	virtual void renderImgui() = 0;

	virtual void destroy();

protected:
	~GFramework() = default;
	inline static GFramework* instance = nullptr;
};
