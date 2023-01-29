#include "Engine.hpp"

#include <ranges>
#include <fstream>

#include "device/window/SDLFramework.hpp"
#include "device/graphics/opengl/OGLFramework.hpp"
#include "input/InputManager.hpp"
#include "ide/ImGuiManager.hpp"

#include "ResourceManager.hpp"

void Engine::init(bool isIDE, char* initFileName)
{
	Engine::isIDE = isIDE;

	ResourceManager::init();
	InputManager::init();
	SDLFramework::init();

	OGLFramework::create();

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
		Engine::event();
		Engine::render();

		if (confirmQuit) break;

		SDLFramework::swapWindow();
	}
}

void Engine::shutDown()
{
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
	OGLFramework::lightSourceCount = 0;
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

	std::vector<uint32_t> inputs = InputManager::triggeredEvents(&Engine::shouldQuit, isIDE);
}

void Engine::render()
{
	OGLFramework::prepareShader(SHADER_BASE);
	OGLFramework::loadCamUniforms(*Engine::activeCam);

	OGLFramework::initRender();

	ResourceManager::backgroundPass();
	ResourceManager::lightPass();
	ResourceManager::modelPass();

	OGLFramework::endRender();
	if (Engine::isIDE) ImGuiManager::render();
}