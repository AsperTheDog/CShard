#pragma once

enum GLibraries
{
	OPENGL = 0,
	VULKAN = 1
};

class GFramework
{
public:
	virtual ~GFramework();

	static void create(GLibraries lib);
	static GFramework* getInstance();

	virtual void init() = 0;
	virtual void loadImGuiBackends() = 0;
	virtual void loadImGuiFrame() = 0;
	virtual void destroyImGui() = 0;

protected:
	inline static GFramework* instance = nullptr;
};