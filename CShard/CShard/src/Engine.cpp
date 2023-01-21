#include "Engine.hpp"

#include <ranges>

#include "device/window/SDLFramework.hpp"
#include "device/graphics/GFramework.hpp"
#include "input/InputManager.hpp"
#include "ide/ImGuiManager.hpp"

#include "device/graphics/GTexture.hpp"
#include "device/graphics/GMesh.hpp"

std::unordered_map<uint32_t, GameObject> Engine::sceneObjects;
Camera* Engine::activeCam;
std::unordered_map<uint32_t, GMesh*> Engine::meshes;
std::unordered_map<uint32_t, GTexture*> Engine::textures;
uint32_t Engine::IDManager;

void Engine::init(GLibraries lib, bool isIDE)
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
}

void Engine::run()
{
	while (true)
	{
		if (Engine::isIDE) ImGuiManager::newFrame();

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

void Engine::compileProject()
{

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

bool Engine::event()
{
	bool shouldClose = false;
	std::vector<uint32_t> inputs =  InputManager::triggeredEvents(&shouldClose, isIDE);

	return shouldClose;
}

void Engine::render()
{
	GFramework::get()->loadCamUniforms(Engine::activeCam);

	GFramework::get()->initRender();
	for (auto& obj : sceneObjects | std::views::values)
	{
		if (obj.hasBackground) obj.processRender();
	}
	for (auto& obj : sceneObjects | std::views::values)
	{
		if (!obj.hasBackground) obj.processRender();
	}
	GFramework::get()->endRender();
	if (Engine::isIDE) ImGuiManager::render();
}