#include "Engine.hpp"

#include "device/window/SDLFramework.hpp"
#include "device/graphics/GFramework.hpp"
#include "ide/ImGuiManager.hpp"
#include "input/InputManager.hpp"

void Engine::init(GLibraries lib)
{
	InputManager::init();

	SDLFramework::init(lib);

	GFramework::create(lib);
	GFramework::get()->init();

	ImGuiManager::init();
	Engine::addImGuiWindows();
}

void Engine::run()
{
	while (true)
	{
		ImGuiManager::newFrame();

		if (Engine::event()) break;
		Engine::render();

		SDLFramework::swapWindow();
	}
}

void Engine::shutDown()
{
	ImGuiManager::destroy();
	GFramework::get()->destroy();
	SDLFramework::destroy();
}

bool Engine::event()
{
	bool shouldClose = false;
	std::vector<uint32_t> inputs =  InputManager::triggeredEvents(&shouldClose);

	return shouldClose;
}

void Engine::render()
{
	GFramework::get()->render();

	ImGuiManager::render();
}

void Engine::addImGuiWindows()
{
	ImGuiManager::addWindowCall(InputManager::ImGuiWindowCall);
}
