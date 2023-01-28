#pragma once
#include <cstdint>

#include "imgui.h"
#include "../../device/graphics/GFramework.hpp"

class PostEffectWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		ImGui::Begin("Post Effects", isOpen, windowFlags);
		ImGui::Checkbox("Activate Post Effects", &GFramework::postEffectsActive);
		ImGui::DragFloat("Color multiplier", &GFramework::postMult, 0.01f, 0.f, 1.f);
		ImGui::Separator();
		ImGui::Text("Film Grain");
		ImGui::End();
	}

private:

};
