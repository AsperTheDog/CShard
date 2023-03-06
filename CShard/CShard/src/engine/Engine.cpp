#include "Engine.hpp"

#include <ranges>
#include <fstream>
#include <iostream>

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

		Collider::init();
	}
	if (initFileName) Engine::loadProject(initFileName);
	if (!activeCam)
	{
		activeCam = &defaultCam;
	}

	if (!Engine::isIDE) Engine::startGame();

	ready = true;
}

void Engine::run()
{
	while (true)
	{
		if (Engine::isIDE) ImGuiManager::newFrame();

		updateDeltaTime();
		for (auto& val : pipelineOrder) pipelineFuncs.at(val)();
		if (isIDE)
			ImGuiManager::update();
		Engine::render();
		
		if (!Engine::isIDE && Engine::shouldQuit) confirmQuit = true;
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
	wf.write((char*) pipelineOrder, sizeof(pipelineOrder));
	ImGuiManager::navigationCam.serialize(wf);
	ResourceManager::save(wf);

	wf.close();
}

void Engine::loadProject(const std::string& filename)
{
	Engine::ready = false;
	resetProject();
	std::ifstream wf("pak/projects/" + filename + ".srdproj", std::ios::in | std::ios::binary);
	if (!wf) {
		SDLFramework::showErrorMessage("Could not load project", "Could not read save file");
		return;
	}
	wf.read((char*) pipelineOrder, sizeof(pipelineOrder));
	ImGuiManager::navigationCam.deserialize(wf);
	ResourceManager::load(wf);

	wf.close();
	Engine::ready = true;
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

void Engine::updateInputs()
{
	InputManager::triggeredEvents(&Engine::shouldQuit, isIDE);
}

void Engine::updateCollisions()
{
	if (isGameRunning)
	{
		CollisionStructure::testCollisions();
	}
}

void Engine::executeScripts()
{
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
	ResourceManager::colliderPass(*activeCam);
	ResourceManager::postPass();
	if (ResourceManager::selectedComponent.second != nullptr && !isGameRunning) 
		ResourceManager::selectedComponent.second->renderWireframe(ResourceManager::selectedComponent.first->modelData, *activeCam);

	GFramework::endRender();
	if (Engine::isIDE) ImGuiManager::render();
}
