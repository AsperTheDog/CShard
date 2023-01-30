#include "ResourceManager.hpp"

#include <fstream>
#include <ranges>

#include "../ide/ImGuiManager.hpp"
#include "../input/InputManager.hpp"

#include "../device/graphics/GFramework.hpp"


void ResourceManager::init()
{
	sceneObjects = std::unordered_map<uint32_t, GameObject>();
	meshes = std::unordered_map<uint32_t, Mesh>();
	textures = std::unordered_map<uint32_t, Texture>();
}

void ResourceManager::reset()
{
	sceneObjects.clear();
	meshes.clear();
	textures.clear();
}

void ResourceManager::load(std::ifstream& wf)
{
	ImGuiManager::navigationCam.deserialize(wf);
	uint32_t num = 0;
	wf.read((char*)&num, sizeof(uint32_t));
	InputManager::inputMappings.reserve(num);
	for (uint32_t i = 0; i < num; i++)
	{
		std::pair<uint32_t, InputMapping> elem;
		wf.read((char*)&elem.first, sizeof(elem));
		InputManager::inputMappings.insert(elem);
	}
	wf.read((char*)&num, sizeof(uint32_t));
	ResourceManager::meshes.reserve(num);
	for (uint32_t i = 0; i < num; i++)
	{
		uint32_t id = 0;
		wf.read((char*)&id, sizeof(id));
		auto elem = meshes.emplace(id, Mesh()).first;
		meshIDCount = std::max(elem->first, meshIDCount);
		char buff[MAX_ASSET_NAME_LENGTH] = "";
		wf.read(buff, MAX_ASSET_NAME_LENGTH);
		elem->second.commit(buff);
	}
	wf.read((char*)&num, sizeof(uint32_t));
	ResourceManager::textures.reserve(num);
	for (uint32_t i = 0; i < num; i++)
	{
		uint32_t id = 0;
		wf.read((char*)&id, sizeof(id));
		auto elem = textures.emplace(id, Texture()).first;
		texIDCount = std::max(elem->first, texIDCount);
		char buff[MAX_ASSET_NAME_LENGTH] = "";
		wf.read(buff, MAX_ASSET_NAME_LENGTH);
		elem->second.commit(buff);
	}
	wf.read((char*)&num, sizeof(uint32_t));
	ResourceManager::sceneObjects.reserve(num);
	for (uint32_t i = 0; i < num; i++)
	{
		uint32_t id = 0;
		wf.read((char*)&id, sizeof(id));
		objIDCount = std::max(id, objIDCount);
		char buff[MAX_PATH_NAME_LENGTH] = "";
		wf.read(buff, MAX_PATH_NAME_LENGTH);
		GameObject obj{ buff };
		wf.read((char*)&obj.modelData, sizeof(obj.modelData));
		obj.modelData.changed = true;
		uint32_t components = 0;
		wf.read((char*)&components, sizeof(components));
		obj.components.reserve(components);
		for (uint32_t j = 0; j < components; j++)
		{
			Component comp{};
			comp.deserialize(wf);
			obj.insertComponent(comp);
		}
		ResourceManager::sceneObjects.emplace(id, obj);
	}
}

void ResourceManager::save(std::ofstream& wf)
{
	ImGuiManager::navigationCam.serialize(wf);
	uint32_t mappingNum = (uint32_t)InputManager::inputMappings.size();
	wf.write((char*)&mappingNum, sizeof(mappingNum));
	for (auto& map : InputManager::inputMappings)
	{
		uint32_t id = map.first;
		wf.write((char*)&id, sizeof(id));
		wf.write((char*)&map.second, sizeof(map.second));
	}
	uint32_t meshNum = (uint32_t)ResourceManager::meshes.size();
	wf.write((char*)&meshNum, sizeof(meshNum));
	for (auto& mesh : ResourceManager::meshes)
	{
		uint32_t id = mesh.first;
		wf.write((char*)&id, sizeof(id));
		char buff[MAX_ASSET_NAME_LENGTH] = "";
		strcpy_s(buff,  sizeof(buff), mesh.second.name.c_str());
		wf.write(buff, MAX_ASSET_NAME_LENGTH);
	}
	uint32_t texNum = (uint32_t)ResourceManager::textures.size();
	wf.write((char*)&texNum, sizeof(texNum));
	for (auto& tex : ResourceManager::textures)
	{
		uint32_t id = tex.first;
		wf.write((char*)&id, sizeof(id));
		char buff[MAX_ASSET_NAME_LENGTH] = "";
		strcpy_s(buff, sizeof(buff), tex.second.name.c_str());
		wf.write(buff, MAX_ASSET_NAME_LENGTH);
	}
	uint32_t objNum = (uint32_t)ResourceManager::sceneObjects.size();
	wf.write((char*)&objNum, sizeof(objNum));
	for (auto& obj : ResourceManager::sceneObjects) {
		uint32_t id = obj.first;
		wf.write((char*)&id, sizeof(id));
		wf.write(obj.second.name, MAX_PATH_NAME_LENGTH);
		wf.write((char*)&obj.second.modelData, sizeof(obj.second.modelData));
		uint32_t components = (uint32_t)obj.second.components.size();
		wf.write((char*)&components, sizeof(components));
		for (auto& comp : obj.second.components) {
			comp.serialize(wf);
		}
	}
}

void ResourceManager::backgroundPass()
{
	for (auto& obj : sceneObjects | std::views::values)
	{
		if (obj.hasBackground) obj.processBackground();
	}
}

void ResourceManager::lightPass()
{
	for (auto& obj : sceneObjects | std::views::values)
	{
		if (obj.lightCount > 0) obj.processLights();
	}
}

void ResourceManager::modelPass(Camera& cam)
{
	for (auto& obj : sceneObjects | std::views::values)
	{
		if (!obj.hasBackground) obj.processModels(cam);
	}
}

uint32_t ResourceManager::addObject()
{
	objIDCount++;
	sceneObjects.emplace(objIDCount, "Object " + std::to_string(objIDCount));
	return objIDCount;
}

uint32_t ResourceManager::addMesh(std::string& filepath)
{
	meshIDCount++;
	auto elem = meshes.emplace(meshIDCount, Mesh()).first;
	elem->second.commit(filepath);
	return meshIDCount;
}

uint32_t ResourceManager::addTexture(std::string& filepath)
{
	texIDCount++;
	auto elem = textures.emplace(texIDCount, Texture()).first;
	elem->second.commit(filepath);
	return texIDCount;
}

void ResourceManager::removeObject(uint32_t id)
{
	if (!sceneObjects.contains(id)) return;
	GFramework::lightSourceCount -= sceneObjects.at(id).lightCount;
	sceneObjects.erase(id);
}

GameObject* ResourceManager::getObject(uint32_t id)
{
	if (!sceneObjects.contains(id)) return nullptr;
	return &sceneObjects.at(id);
}

bool ResourceManager::isValidMesh(uint32_t id)
{
	return meshes.contains(id);
}

Mesh* ResourceManager::getMesh(uint32_t id)
{
	if (!isValidMesh(id)) return nullptr;
	return &meshes.at(id);
}

bool ResourceManager::isValidTexture(uint32_t id)
{
	return textures.contains(id);
}

Texture* ResourceManager::getTexture(uint32_t id)
{
	if (!isValidTexture(id)) return nullptr;
	return &textures.at(id);
}

void ResourceManager::clone(uint32_t index)
{
	if (!sceneObjects.contains(index)) return;
	GameObject objToCopy = sceneObjects.at(index);
	GameObject newObj{objToCopy.name};
	newObj.modelData = objToCopy.modelData;
	for (auto& comp : objToCopy.components)
	{
		Component newComp{};
		memcpy(&newComp, &comp, sizeof(comp));
		newObj.insertComponent(comp);
	}
	objIDCount++;
	sceneObjects.emplace(objIDCount, newObj);
}