#pragma once

#include <vector>

#include "device/graphics/GUtils.hpp"
#include "elements/GameObject.hpp"

class Engine
{
public:
	Engine() = delete;

	static void init(GLibraries lib, bool isIDE);
	static void run();
	static void shutDown();
	static void compileProject();

	inline static bool isIDE;
private:
	static bool event();
	static void render();

	static std::vector<GameObject> sceneObjects;
};

