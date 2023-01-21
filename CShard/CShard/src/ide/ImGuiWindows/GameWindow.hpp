#pragma once

#include "../../device/graphics/GFramework.hpp"

class GameWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGui::Begin("Game", isOpen);
		GFramework::imGuiSize = {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y};
		ImGui::Image(
			(ImTextureID)GFramework::get()->getImGuiTexture(), 
			ImGui::GetContentRegionAvail(), 
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}

private:

};

