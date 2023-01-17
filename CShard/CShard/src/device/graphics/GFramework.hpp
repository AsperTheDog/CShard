#pragma once

enum GLibraries
{
	OPENGL = 0,
	VULKAN = 1
};

class GFramework
{
public:
	static void create(GLibraries lib);
	static GFramework* getInstance();
	static void deleteInstance();

	virtual void init() = 0;
	virtual void loadImGuiBackends() = 0;
	virtual void loadImGuiFrame() = 0;
	virtual void destroyImGui() = 0;

protected:
	inline static GFramework* instance = nullptr;
};