#pragma once
#include <vector>

#include "imgui.h"

typedef void (*ImGuiWindowCall)();

class ImGuiManager
{
public:
	ImGuiManager() = delete;

	static void init();
	static void newFrame();
	static void render();
	static void destroy();

	static void addWindowCall(ImGuiWindowCall call);
private:
	static std::vector<ImGuiWindowCall> windowCalls;
};

