#pragma once
#include <string>
#include <vector>

#include <imgui.h>

#include "../elements/components/Camera.hpp"
#include "../elements/components/Model.hpp"

typedef void (*ImGuiWindowCall)(bool*);

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
	static void destroy();
	static ImGuiIO* getIO();

	static void addImGuiWindows();
	static void addWindowCall(ImGuiWindowCall call, std::string name, bool defaultOpen);
	static void updateSceneCam();

	inline static Camera navigationCam{};
private:
	inline static std::vector<WindowData> windowCalls{};
	inline static ImGuiIO* io{};
};

