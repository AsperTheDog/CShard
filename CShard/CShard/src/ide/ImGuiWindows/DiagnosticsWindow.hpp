#pragma once
#include <imgui.h>

class DiagnosticsWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		ImGui::Begin("Diagnostics", isOpen, windowFlags);
		ImGui::Text("Framerate:");
		ImGui::Text("\tLast: %f", 1.f / ImGui::GetIO().DeltaTime);
		ImGui::Text("\tAverage: %f", ImGui::GetIO().Framerate);
		ImGui::Separator();
		ImGui::Text("Graphics library: OpenGL");
		ImGui::End();
	}

private:

};

