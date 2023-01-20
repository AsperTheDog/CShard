#pragma once
#include "imgui.h"

class DiagnosticsWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGui::Begin("Logs", isOpen);
		ImGui::Text("Framerate:");
		ImGui::Text("\tLast: %f", 1.f/ImGui::GetIO().DeltaTime);
		ImGui::Text("\tAverage: %f", ImGui::GetIO().Framerate);
		ImGui::End();
	}

private:

};

