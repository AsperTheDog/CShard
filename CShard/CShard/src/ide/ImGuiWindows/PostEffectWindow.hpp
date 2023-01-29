#pragma once

#include "imgui.h"

class PostEffectWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		ImGui::Begin("Post Effects", isOpen, windowFlags);
		ImGui::Checkbox("Activate Post Effects", &OGLFramework::postEffectsActive);
		ImGui::DragFloat("Color multiplier", &OGLFramework::postMult, 0.01f, 0.f, 1.f);
		ImGui::Separator();
		ImGui::Text("Film Grain");
		ImGui::DragFloat("Intensity", &OGLFramework::filmGrain.intensity, 0.01f, 0.01f, 1.f);
		ImGui::End();
	}

private:

};
