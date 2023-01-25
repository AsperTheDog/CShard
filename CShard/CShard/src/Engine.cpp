#include "Engine.hpp"

#include <ranges>
#include <fstream>

#include "device/window/SDLFramework.hpp"
#include "device/graphics/GFramework.hpp"
#include "input/InputManager.hpp"
#include "ide/ImGuiManager.hpp"

#include "device/graphics/GTexture.hpp"
#include "device/graphics/GMesh.hpp"

#include "Config.hpp"

void Engine::init(GLibraries lib, bool isIDE, char* initFileName)
{
	Engine::isIDE = isIDE;
	Engine::sceneObjects = std::unordered_map<uint32_t, GameObject>();
	Engine::meshes = std::unordered_map<uint32_t, GMesh*>();
	Engine::textures = std::unordered_map<uint32_t, GTexture*>();

	InputManager::init();

	SDLFramework::init(lib);

	GFramework::create(lib);

	if (Engine::isIDE)
	{
		ImGuiManager::init();
		ImGuiManager::addImGuiWindows();
		activeCam = &ImGuiManager::navigationCam;
	}
	if (initFileName) Engine::loadProject(initFileName);
	if (!activeCam)
	{
		activeCam = &defaultCam;
	}
}

void Engine::run()
{
	while (true)
	{
		if (Engine::isIDE) ImGuiManager::newFrame();

		updateDeltaTime();
		if (Engine::event()) break;
		Engine::render();

		SDLFramework::swapWindow();
	}
}

void Engine::shutDown()
{
	for (auto mesh : meshes | std::views::values) delete mesh;
	for (auto texture : textures | std::views::values) delete texture;
	if (Engine::isIDE) ImGuiManager::destroy();
	GFramework::deleteInstance();
	SDLFramework::destroy();
}

void Engine::compileProject(const std::string& name)
{
	std::ofstream wf("pak/projects/" + name + ".srdproj", std::ios::out | std::ios::binary);
	if (!wf) {
		SDLFramework::showErrorMessage("Could not save project", "Could not write to save file");
		return;
	}
	wf.write((char*) &ImGuiManager::navigationCam, sizeof(Camera));
	uint32_t mappingNum = (uint32_t)InputManager::inputMappings.size();
	wf.write((char*)&mappingNum, sizeof(mappingNum));
	for (auto& map : InputManager::inputMappings)
	{
		uint32_t id = map.first;
		wf.write((char*)&id, sizeof(id));
		wf.write((char*)&map.second, sizeof(map.second));
	}
	uint32_t meshNum = (uint32_t)meshes.size();
	wf.write((char*)&meshNum, sizeof(meshNum));
	for (auto& mesh : meshes)
	{
		uint32_t id = mesh.first;
		wf.write((char*)&id, sizeof(id));
		char buff[MAX_ASSET_NAME_LENGTH] = "";
		strcpy_s(buff,  sizeof(buff), mesh.second->name.c_str());
		wf.write(buff, MAX_ASSET_NAME_LENGTH);
	}
	uint32_t texNum = (uint32_t)textures.size();
	wf.write((char*)&texNum, sizeof(texNum));
	for (auto& tex : textures)
	{
		uint32_t id = tex.first;
		wf.write((char*)&id, sizeof(id));
		char buff[MAX_ASSET_NAME_LENGTH] = "";
		strcpy_s(buff, sizeof(buff), tex.second->name.c_str());
		wf.write(buff, MAX_ASSET_NAME_LENGTH);
	}
	uint32_t objNum = (uint32_t)sceneObjects.size();
	wf.write((char*)&objNum, sizeof(objNum));
	for (auto& obj : sceneObjects) {
		uint32_t id = obj.first;
		wf.write((char*)&id, sizeof(id));
		wf.write(obj.second.name, MAX_OBJ_NAME_LENGTH);
		wf.write((char*)&obj.second.modelData, sizeof(obj.second.modelData));
		uint32_t components = (uint32_t)obj.second.components.size();
		wf.write((char*)&components, sizeof(components));
		for (auto& comp : obj.second.components) {
			wf.write((char*)&comp, sizeof(comp));
		}
	}
	
	wf.close();
}

void Engine::loadProject(std::string filename)
{
	InputManager::inputMappings.clear();
	Engine::sceneObjects.clear();
	Engine::meshes.clear();
	Engine::textures.clear();
	std::ifstream wf("pak/projects/" + filename + ".srdproj", std::ios::in | std::ios::binary);
	if (!wf) {
		SDLFramework::showErrorMessage("Could not load project", "Could not read save file");
		return;
	}
	wf.read((char*) &ImGuiManager::navigationCam, sizeof(Camera));
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
	meshes.reserve(num);
	for (uint32_t i = 0; i < num; i++)
	{
		std::pair<uint32_t, GMesh*> elem;
		wf.read((char*)&elem.first, sizeof(elem.first));
		char buff[MAX_ASSET_NAME_LENGTH] = "";
		wf.read(buff, MAX_ASSET_NAME_LENGTH);
		elem.second = GFramework::get()->createMesh(buff);
		meshes.insert(elem);
	}
	wf.read((char*)&num, sizeof(uint32_t));
	textures.reserve(num);
	for (uint32_t i = 0; i < num; i++)
	{
		std::pair<uint32_t, GTexture*> elem;
		wf.read((char*)&elem.first, sizeof(elem.first));
		char buff[MAX_ASSET_NAME_LENGTH] = "";
		wf.read(buff, MAX_ASSET_NAME_LENGTH);
		elem.second = GFramework::get()->createTexture(buff);
		textures.insert(elem);
	}
	wf.read((char*)&num, sizeof(uint32_t));
	sceneObjects.reserve(num);
	for (uint32_t i = 0; i < num; i++)
	{
		uint32_t id = 0;
		wf.read((char*)&id, sizeof(id));
		char buff[MAX_OBJ_NAME_LENGTH] = "";
		wf.read(buff, MAX_OBJ_NAME_LENGTH);
		GameObject obj{ buff };
		wf.read((char*)&obj.modelData, sizeof(obj.modelData));
		uint32_t components = 0;
		wf.read((char*)&components, sizeof(components));
		obj.components.reserve(components);
		for (uint32_t j = 0; j < components; j++)
		{
			Component comp{};
			wf.read((char*)&comp, sizeof(comp));
			if (comp.type == COMPONENT_BACKGROUND) obj.hasBackground = true;
			else if (comp.type == COMPONENT_LIGHT) GFramework::lightSourceCount++;
			obj.components.push_back(comp);
		}
		sceneObjects.emplace(id, obj);
	}

	wf.close();
}

uint32_t Engine::addObject()
{
	IDManager++;
	Engine::sceneObjects.emplace(IDManager, "Object " + std::to_string(IDManager));
	return IDManager;
}

uint32_t Engine::addMesh(std::string& filepath)
{
	IDManager++;
	Engine::meshes.emplace(IDManager, GFramework::get()->createMesh(filepath));
	return IDManager;
}

uint32_t Engine::addTexture(std::string& filepath)
{
	IDManager++;
	Engine::textures.emplace(IDManager, GFramework::get()->createTexture(filepath));
	return IDManager;
}

void Engine::removeObject(uint32_t id)
{
	if (!sceneObjects.contains(id)) return;
	Engine::sceneObjects.erase(id);
}

GameObject* Engine::getObject(uint32_t id)
{
	if (!sceneObjects.contains(id)) return nullptr;
	return &Engine::sceneObjects.at(id);
}

bool Engine::isValidMesh(uint32_t id)
{
	return meshes.contains(id);
}

GMesh* Engine::getMesh(uint32_t id)
{
	if (!isValidMesh(id)) return nullptr;
	return meshes.at(id);
}

bool Engine::isValidTexture(uint32_t id)
{
	return textures.contains(id);
}

GTexture* Engine::getTexture(uint32_t id)
{
	if (!isValidTexture(id)) return nullptr;
	return textures.at(id);
}

void Engine::updateDeltaTime()
{
	uint64_t now = SDLFramework::getWindowTime();
	dt = (float)(now - prevDt) * 0.001f;
	prevDt = now;
}

bool Engine::event()
{
	if (isIDE)
		ImGuiManager::update();

	bool shouldClose = false;
	std::vector<uint32_t> inputs = InputManager::triggeredEvents(&shouldClose, isIDE);

	return shouldClose;
}

void Engine::render()
{
	GFramework::get()->loadCamUniforms(*Engine::activeCam);

	GFramework::get()->initRender();
	for (auto& obj : sceneObjects | std::views::values)
	{
		if (obj.hasBackground) obj.processRender();
	}
	for (auto& obj : sceneObjects | std::views::values)
	{
		if (!obj.hasBackground) obj.processLights();
	}
	for (auto& obj : sceneObjects | std::views::values)
	{
		if (!obj.hasBackground) obj.processRender();
	}
	GFramework::get()->endRender();
	if (Engine::isIDE) ImGuiManager::render();
}