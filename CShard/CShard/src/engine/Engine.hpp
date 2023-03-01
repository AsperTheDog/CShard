#pragma once

#include <unordered_map>

#include "../elements/components/Camera.hpp"

class PostEffect;
class GTexture;
class GMesh;

enum PipelineElement
{
	PIPELINE_INPUT,
	PIPELINE_SCRIPT,
	PIPELINE_COLLISION
};

typedef void (*PipelineFunc)();

class Engine
{
public:
	Engine() = delete;

	static void init(bool isIDE, char* initFileName = nullptr);
	static void run();
	static void shutDown();
	static void compileProject(const std::string& name);
	static void loadProject(const std::string& filename);
	static void resetProject();
	static void startGame();
	static void stopGame();
	inline static Camera* activeCam;

	inline static bool isIDE = false;
	inline static float dt = 0.f;
	inline static bool shouldQuit = false;
	inline static bool confirmQuit = false;
	inline static bool isGameRunning = false;
	inline static bool ready = false;

	inline static PipelineElement pipelineOrder[3] = {
		PIPELINE_INPUT,
		PIPELINE_SCRIPT,
		PIPELINE_COLLISION
	};

private:
	static void updateDeltaTime();

	static void updateInputs();
	static void updateCollisions();
	static void executeScripts();

	static void render();

	inline static Camera defaultCam{};
	inline static uint64_t prevDt = 0;

	inline static uint32_t IDManager = 0;

	inline static std::unordered_map<PipelineElement, PipelineFunc> pipelineFuncs = {
		{PIPELINE_COLLISION, Engine::updateCollisions},
		{PIPELINE_INPUT, Engine::updateInputs},
		{PIPELINE_SCRIPT, Engine::executeScripts}
	};
};

