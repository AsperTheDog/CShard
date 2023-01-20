#pragma once
#include <string>
#include <vector>

#include "imgui.h"

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
private:
	static std::vector<WindowData> windowCalls;
	static ImGuiIO* io;
};

