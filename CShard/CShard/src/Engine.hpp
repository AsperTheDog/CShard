#pragma once

#include <unordered_map>

#include "device/graphics/GUtils.hpp"
#include "elements/GameObject.hpp"

#define MAX_LIGHTS 10

class Camera;
class GTexture;
class GMesh;

class Engine
{
public:
	Engine() = delete;

	static void init(GLibraries lib, bool isIDE);
	static void run();
	static void shutDown();
	static void compileProject();
	static uint32_t addObject();
	static uint32_t addMesh(std::string& filepath);
	static uint32_t addTexture(std::string& filepath);
	static void removeObject(uint32_t id);
	static GameObject* getObject(uint32_t id);
	static bool isValidMesh(uint32_t id);
	static GMesh* getMesh(uint32_t id);
	static bool isValidTexture(uint32_t id);
	static GTexture* getTexture(uint32_t id);

	static std::unordered_map<uint32_t, GameObject> sceneObjects;
	static std::unordered_map<uint32_t, GMesh*> meshes;
	static std::unordered_map<uint32_t, GTexture*> textures;
	static Camera* activeCam;

	inline static bool isIDE;
	inline static float dt = 0.f;
private:
	static void updateDeltaTime();
	static bool event();
	static void render();

	inline static uint64_t prevDt = 0;

	static uint32_t IDManager;
};

