#pragma once

#include "../../engine/Engine.hpp"

class SceneWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGui::Begin("Game", isOpen);
		if (ImGui::Button("Run"))
		{
			Engine::startGame();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			Engine::stopGame();
		}
		GFramework::imGuiSize = {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y};
		ImGui::Image(
			(ImTextureID)(uint64_t)GFramework::getImGuiTexture(), 
			ImGui::GetContentRegionAvail(), 
			ImVec2(0, 1), ImVec2(1, 0));
		isFocused = ImGui::IsWindowFocused();
		ImGui::End();
	}
	
	inline static bool isFocused = false;
private:

};

