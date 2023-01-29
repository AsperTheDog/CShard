#pragma once

#include <unordered_map>

#include "device/graphics/GUtils.hpp"
#include "elements/GameObject.hpp"
#include "elements/components/Camera.hpp"

class GTexture;
class GMesh;

class Engine
{
public:
	Engine() = delete;

	static void init(GLibraries lib, bool isIDE, char* initFileName = nullptr);
	static void run();
	static void shutDown();
	static void compileProject(const std::string& name);
	static void loadProject(const std::string& filename);
	static void resetProject();
	inline static Camera* activeCam;

	inline static bool isIDE = false;
	inline static float dt = 0.f;
	inline static bool shouldQuit = false;
	inline static bool confirmQuit = false;

private:
	static void updateDeltaTime();
	static void event();
	static void render();

	inline static Camera defaultCam{};
	inline static uint64_t prevDt = 0;

	inline static uint32_t IDManager = 0;
};

