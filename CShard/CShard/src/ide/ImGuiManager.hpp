#pragma once
#include <vector>

#include "imgui.h"

typedef void (*ImGuiWindowCall)(bool*);

class ImGuiManager
{
public:
	ImGuiManager() = delete;

	static void init();
	static void newFrame();
	static void render();
	static void destroy();
	static ImGuiIO* getIO();

	static void addWindowCall(ImGuiWindowCall call);
private:
	static std::vector<std::pair<ImGuiWindowCall, bool>> windowCalls;
	static ImGuiIO* io;
};

