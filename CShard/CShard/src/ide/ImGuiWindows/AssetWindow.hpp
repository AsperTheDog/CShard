#pragma once
#include <imgui.h>
#include "../../Engine.hpp"

#include "../../device/graphics/GTexture.hpp"
#include "../../device/graphics/GMesh.hpp"

class AssetWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable;
		ImGui::Begin("Meshes", isOpen);
		if (ImGui::BeginTabBar("Assets"))
            {
                if (ImGui::BeginTabItem("Meshes"))
                {
					ImGui::InputText("Path##Mesh", buff, 200);
					ImGui::SameLine();
					if(ImGui::Button("Import##Mesh"))
					{
						
					}
					ImGui::Separator();
					ImGui::BeginTable("Meshes##Mesh", 3, flags);
					ImGui::TableSetupColumn("ID##Mesh", ImGuiTableColumnFlags_WidthFixed);
				    ImGui::TableSetupColumn("Path##Mesh", ImGuiTableColumnFlags_WidthStretch);
				    ImGui::TableHeadersRow();
				    for (auto& mesh : Engine::meshes)
				    {
				        ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%d", mesh.first);
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(mesh.second->name.c_str());
				    }
				    ImGui::EndTable();
					ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Textures"))
                {
                    ImGui::InputText("Path##Tex", buff, 200);
					ImGui::SameLine();
					if(ImGui::Button("Import##Tex"))
					{
						
					}
					ImGui::Separator();
					ImGui::BeginTable("Texture##Tex", 3, flags);
					ImGui::TableSetupColumn("ID##Tex", ImGuiTableColumnFlags_WidthFixed);
				    ImGui::TableSetupColumn("Path##Tex", ImGuiTableColumnFlags_WidthStretch);
				    ImGui::TableHeadersRow();
				    for (auto& tex : Engine::textures)
				    {
				        ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%d", tex.first);
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(tex.second->name.c_str());
				    }
				    ImGui::EndTable();
					ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
		ImGui::End();
	}

private:
	inline static char buff[200] = "";
};
