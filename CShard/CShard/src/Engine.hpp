#pragma once

#include <unordered_map>
#include <vec2.hpp>

#include "device/graphics/GUtils.hpp"
#include "elements/GameObject.hpp"

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
private:
	static bool event();
	static void render();

	static uint32_t IDManager;
};

