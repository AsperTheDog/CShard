#pragma once
#include <imgui.h>
#include "../../device/graphics/GFramework.hpp"

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
		ImGui::Text("Graphics library: %s", GFramework::type == OPENGL ? "OpenGL" : "Vulkan");
		ImGui::End();
	}

private:

};

