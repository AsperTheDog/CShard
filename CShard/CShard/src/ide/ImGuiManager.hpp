#pragma once
#include "imgui.h"

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	void newFrame();
	void render();

private:
	ImGuiIO* io;
};

