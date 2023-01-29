#pragma once
#include <unordered_map>

#include "../device/graphics/Mesh.hpp"
#include "../device/graphics/Texture.hpp"
#include "../elements/GameObject.hpp"

class ResourceManager
{
public:
	static void init();
	static void reset();

	static void load(std::ifstream& wf);
	static void save(std::ofstream& wf);

	static void backgroundPass();
	static void lightPass();
	static void modelPass();

	static uint32_t addObject();
	static uint32_t addMesh(std::string& filepath);
	static uint32_t addTexture(std::string& filepath);
	static void removeObject(uint32_t id);
	static GameObject* getObject(uint32_t id);
	static bool isValidMesh(uint32_t id);
	static Mesh* getMesh(uint32_t id);
	static bool isValidTexture(uint32_t id);
	static Texture* getTexture(uint32_t id);
	static void clone(uint32_t index);

	inline static std::unordered_map<uint32_t, GameObject> sceneObjects{};
	inline static std::unordered_map<uint32_t, Mesh> meshes{};
	inline static std::unordered_map<uint32_t, Texture> textures{};

private:
	inline static uint32_t meshIDCount = 0;
	inline static uint32_t texIDCount = 0;
	inline static uint32_t objIDCount = 0;
};

