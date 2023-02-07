#include "Engine.hpp"

#include <ranges>
#include <fstream>

#include "../device/window/SDLFramework.hpp"
#include "../device/graphics/GFramework.hpp"
#include "../input/InputManager.hpp"
#include "../ide/ImGuiManager.hpp"

#include "../elements/components/Script.hpp"

#include "ResourceManager.hpp"
#include "lua/Manager.hpp"

void Engine::init(bool isIDE, char* initFileName)
{
	Engine::isIDE = isIDE;

	ResourceManager::init();
	InputManager::init();
	SDLFramework::init();
	CSLua::Manager::init();

	GFramework::create();

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

	if (!Engine::isIDE) isGameRunning = true;
}

void Engine::run()
{
	while (true)
	{
		if (Engine::isIDE) ImGuiManager::newFrame();

		updateDeltaTime();
		Engine::event();
		Engine::render();

		if (confirmQuit) break;

		SDLFramework::swapWindow();
	}
}

void Engine::shutDown()
{
	if (Engine::isGameRunning) Engine::stopGame();
	if (Engine::isIDE) ImGuiManager::destroy();
	SDLFramework::destroy();
}

void Engine::compileProject(const std::string& name)
{
	std::ofstream wf("pak/projects/" + name + ".srdproj", std::ios::out | std::ios::binary);
	if (!wf) {
		SDLFramework::showErrorMessage("Could not save project", "Could not write to save file");
		return;
	}
	ResourceManager::save(wf);

	wf.close();
}

void Engine::loadProject(const std::string& filename)
{
	resetProject();
	std::ifstream wf("pak/projects/" + filename + ".srdproj", std::ios::in | std::ios::binary);
	if (!wf) {
		SDLFramework::showErrorMessage("Could not load project", "Could not read save file");
		return;
	}
	ResourceManager::load(wf);

	wf.close();
}

void Engine::resetProject()
{
	InputManager::inputMappings.clear();
	ResourceManager::reset();
	GFramework::lightSourceCount = 0;
	bool wasGameRunning = isGameRunning;
	Engine::stopGame();
	if (wasGameRunning) Engine::startGame();
}

void Engine::startGame()
{
	if (isGameRunning) Engine::stopGame();
	for (auto& comp : ResourceManager::sceneObjects)
	{
		comp.second.processScripts(comp.first, ScriptType::SCRIPT_INIT);
	}
	isGameRunning = true;
}

void Engine::stopGame()
{
	for (auto& comp : ResourceManager::sceneObjects)
	{
		comp.second.processScripts(comp.first, ScriptType::SCRIPT_DIE);
	}
	isGameRunning = false;
	Engine::activeCam = &ImGuiManager::navigationCam;
	CSLua::Manager::reset();
}

void Engine::updateDeltaTime()
{
	uint64_t now = SDLFramework::getWindowTime();
	dt = (float)(now - prevDt) * 0.001f;
	prevDt = now;
}

void Engine::event()
{
	if (isIDE)
		ImGuiManager::update();

	InputManager::triggeredEvents(&Engine::shouldQuit, isIDE);
	if (isGameRunning)
	{
		for (auto& comp : ResourceManager::sceneObjects)
		{
			comp.second.processScripts(comp.first, ScriptType::SCRIPT_FRAME);
		}
	}

}

void Engine::render()
{
	GFramework::prepareShader(SHADER_BASE);
	GFramework::loadCamUniforms(*Engine::activeCam);

	GFramework::initRender();

	ResourceManager::backgroundPass();
	ResourceManager::lightPass();
	ResourceManager::modelPass(*activeCam);
	ResourceManager::postPass();

	GFramework::endRender();
	if (Engine::isIDE) ImGuiManager::render();
}