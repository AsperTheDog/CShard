#pragma once

#include "imgui.h"
#include "../../device/graphics/PostEffects.hpp"

class PostEffectWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		ImGui::Begin("Post effects", isOpen, windowFlags);
		ImGui::Combo("Input type", &tempEffect, names, 6);
		if (ImGui::Button("Create"))
		{
			ResourceManager::addPostEffect(types[tempEffect]);
		}
		ImGui::Separator();
		ImGui::BeginDisabled(selected <= 0);
		if (ImGui::Button("Up"))
		{
			std::swap(ResourceManager::posts.at(selected), ResourceManager::posts.at(selected - 1));
			selected--;
		}
		ImGui::EndDisabled();
		ImGui::SameLine();
		ImGui::BeginDisabled(ResourceManager::posts.empty() || selected >= ResourceManager::posts.size() - 1);
		if (ImGui::Button("Down"))
		{
			std::swap(ResourceManager::posts.at(selected), ResourceManager::posts.at(selected + 1));
			selected++;
		}
		ImGui::EndDisabled();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Separator();
		bool shouldRemove = false;
		uint32_t removeIdx = 0;
		for (int n = 0; n < ResourceManager::posts.size(); n++)
            {
                PostEffect* item = ResourceManager::posts.at(n);
				if (ImGui::Button(("-##" + std::to_string(n)).c_str()))
				{
					shouldRemove = true;
					removeIdx = n;
				}
				ImGui::SameLine();
                if (ImGui::Selectable((std::to_string(n) + ": " + item->getName()).c_str(), selected == n))
                {
	                selected = n;
                }
				ImGui::Separator();
                
				item->showImGuiWindow(n);
				ImGui::Separator();
				ImGui::Separator();
				ImGui::Separator();
            }
		if (shouldRemove)
		{
			ResourceManager::removePostEffect(removeIdx);
			if (removeIdx <= selected && selected != 0) selected--;
		}
		ImGui::End();
	}

private:
	inline static int tempEffect = 0;
    inline static const char* names[] = {
        "film grain",
		"atmospheric fog",
		"fade to black",
		"depth buffer",
		"pixelate",
		"color correction"
    };
	inline static PostType types[] = {
        POST_FILMGRAIN,
		POST_ATMOSPHERICFOG,
		POST_FADETOBLACK,
		POST_DEPTHBUFFER,
		POST_PIXELATE,
		POST_CORRECTION
    };

	inline static int selected = 0;
};
