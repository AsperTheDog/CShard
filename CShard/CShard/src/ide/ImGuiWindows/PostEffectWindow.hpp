#pragma once

#include "imgui.h"

class PostEffectWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		
		if (!*isOpen) return;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		ImGui::Begin("Post effects", isOpen, windowFlags);
		for (uint32_t id = 0; id < GFramework::posts.size(); id++)
		{
			GFramework::posts.at(id)->showImGuiWindow(id);
		}
		ImGui::End();
	}

private:

};
