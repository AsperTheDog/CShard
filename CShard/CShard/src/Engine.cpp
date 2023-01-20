#include "Engine.hpp"

#include "device/window/SDLFramework.hpp"
#include "device/graphics/GFramework.hpp"
#include "ide/ImGuiManager.hpp"
#include "input/InputManager.hpp"

std::vector<GameObject> Engine::sceneObjects;

void Engine::init(GLibraries lib, bool isIDE)
{
	Engine::isIDE = isIDE;
	Engine::sceneObjects = std::vector<GameObject>();

	InputManager::init();

	SDLFramework::init(lib);

	GFramework::create(lib);
	GFramework::get()->init();

	if (Engine::isIDE)
	{
		ImGuiManager::init();
		ImGuiManager::addImGuiWindows();
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
	if (Engine::isIDE) ImGuiManager::destroy();
	GFramework::deleteInstance();
	SDLFramework::destroy();
}

void Engine::compileProject()
{

}

bool Engine::event()
{
	bool shouldClose = false;
	std::vector<uint32_t> inputs =  InputManager::triggeredEvents(&shouldClose);

	return shouldClose;
}

void Engine::render()
{
	if (Engine::isIDE) ImGuiManager::render();
}