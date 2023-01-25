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
	static void loadProject(std::string filename);
	static uint32_t addObject();
	static uint32_t addMesh(std::string& filepath);
	static uint32_t addTexture(std::string& filepath);
	static void removeObject(uint32_t id);
	static GameObject* getObject(uint32_t id);
	static bool isValidMesh(uint32_t id);
	static GMesh* getMesh(uint32_t id);
	static bool isValidTexture(uint32_t id);
	static GTexture* getTexture(uint32_t id);

	inline static std::unordered_map<uint32_t, GameObject> sceneObjects{};
	inline static std::unordered_map<uint32_t, GMesh*> meshes{};
	inline static std::unordered_map<uint32_t, GTexture*> textures{};
	inline static Camera* activeCam;

	inline static bool isIDE = false;
	inline static float dt = 0.f;

private:
	static void updateDeltaTime();
	static bool event();
	static void render();

	inline static Camera defaultCam{};
	inline static uint64_t prevDt = 0;

	inline static uint32_t IDManager = 0;
};

