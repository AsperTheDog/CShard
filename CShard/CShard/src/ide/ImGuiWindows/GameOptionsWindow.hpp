#pragma once
#include <imgui.h>

class GameOptionsWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		ImGui::Begin("Display options", isOpen, windowFlags);

		ImGui::End();
	}
};

