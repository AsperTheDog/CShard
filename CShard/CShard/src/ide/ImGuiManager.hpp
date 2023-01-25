#pragma once
#include <string>
#include <vector>

#include <imgui.h>
#include <unordered_map>

#include "../elements/components/Camera.hpp"
#include "../elements/components/Model.hpp"

#include "../Config.hpp"

typedef void (*ImGuiWindowCall)(bool*);

enum SceneControls
{
	FORWARD = 'w',
	LEFT = 'a',
	BACKWARD = 's',
	RIGHT = 'd'
};

class ImGuiManager
{
public:
	struct WindowData
	{
		std::string name;
		ImGuiWindowCall window;
		bool isOpen;

		WindowData(std::string name, ImGuiWindowCall window, bool isOpen)
		: name(std::move(name)), window(window), isOpen(isOpen){}
	};

	ImGuiManager() = delete;

	static void init();
	static void newFrame();
	static void render();
	static void update();
	static void destroy();
	static ImGuiIO* getIO();

	static void addImGuiWindows();
	static void addWindowCall(ImGuiWindowCall call, std::string name, bool defaultOpen);
	static void updateSceneCamDir();
	static void updateSceneCamPos();

	inline static Camera navigationCam{};
	inline static glm::vec2 mousePos{};
	inline static glm::vec2 lastMousePos{};
	inline static std::unordered_map<int32_t, bool> keyDowns{};
	inline static float movementMult = 6.f;
private:

	inline static char nameBuff[MAX_OBJ_NAME_LENGTH];
	inline static bool showSaveWin = false;
	inline static std::vector<WindowData> windowCalls{};
	inline static ImGuiIO* io{};
};

