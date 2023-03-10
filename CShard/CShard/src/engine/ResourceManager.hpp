#pragma once
#include <map>
#include <FileWatch.hpp>
#include <filesystem>

#include "../device/graphics/Mesh.hpp"
#include "../device/graphics/Texture.hpp"
#include "../elements/GameObject.hpp"
#include "lua/LuaScript.hpp"
#include "../device/graphics/PostEffectTypes.hpp"

class PostEffect;

class ResourceManager
{
	struct AssetRef
	{
		union AssetPointer
		{
			Mesh* mesh;
			Texture* tex;
			LuaScript* scr;
		};
		AssetPath::AssetType type;
		uint32_t id;
		AssetPointer ptr;
	};

	static void watcherCallback(const std::string& path, const filewatch::Event event);

public:
	static void init();
	static void reset();

	static void load(std::ifstream& wf);
	static void save(std::ofstream& wf);

	static void backgroundPass();
	static void lightPass();
	static void modelPass(Camera& cam);
	static void colliderPass(Camera& cam);

	static uint32_t addObject();
	static uint32_t addMesh(std::string& filepath);
	static uint32_t addTexture(std::string& filepath);
	static uint32_t addScript(std::string& filepath);
	static void removeMesh(uint32_t id);
	static void removeTexture(uint32_t id);
	static void removeScript(uint32_t id);
	static void removeObject(uint32_t id);
	static GameObject* getObject(uint32_t id);
	static bool isValidMesh(uint32_t id);
	static Mesh* getMesh(uint32_t id);
	static bool isValidTexture(uint32_t id);
	static Texture* getTexture(uint32_t id);
	static void clone(uint32_t index);
	static bool isValidScript(uint32_t id);
	static LuaScript* getScript(uint32_t id);
	static void addPostEffect(PostType post);
	static void removePostEffect(uint32_t elem);
	static void postPass();
	static PostEffect* getPost(uint32_t pst);
	static AssetRef getFileFromPath(std::filesystem::path& path);

	inline static std::map<uint32_t, GameObject> sceneObjects{};
	inline static std::map<uint32_t, Mesh> meshes{};
	inline static std::map<uint32_t, Texture> textures{};
	inline static std::map<uint32_t, LuaScript> scripts{};
	inline static std::vector<PostEffect*> posts{};

	inline static std::pair<GameObject*, Component*> selectedComponent{ nullptr, nullptr };
	
	inline static AssetRef toReload{AssetPath::AssetType::ERR};
	inline static filewatch::FileWatch<std::string> watch{"./pak/", watcherCallback};

private:
	
	inline static AssetRef renamed{AssetPath::AssetType::ERR};
	inline static uint32_t meshIDCount = 0;
	inline static uint32_t texIDCount = 0;
	inline static uint32_t objIDCount = 0;
	inline static uint32_t scrIDCount = 0;
};

