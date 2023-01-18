#pragma once

#include "device/graphics/GUtils.hpp"

class Engine
{
public:
	Engine() = delete;

	static void init(GLibraries lib);
	static void run();
	static void shutDown();

private:
	static bool event();
	static void render();

	static void addImGuiWindows();
};

