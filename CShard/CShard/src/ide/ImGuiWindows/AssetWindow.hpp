#pragma once
#include <imgui.h>
#include <stdexcept>

#include "../../engine/ResourceManager.hpp"
#include "../../device/graphics/Mesh.hpp"
#include "../../device/graphics/Texture.hpp"

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
					ImGui::InputText("Path##Mesh", meshBuff, MAX_ASSET_NAME_LENGTH);
					ImGui::SameLine();
					if(ImGui::Button("Import##Mesh"))
					{
						std::string path = {meshBuff};
						try
						{
							ResourceManager::addMesh(path);
						}
						catch (std::runtime_error&)
						{
							SDLFramework::showErrorMessage("Could not load mesh", "Make sure the path is correct");
						}

					}
					ImGui::Separator();
					ImGui::BeginTable("Script##Mesh", 4, flags);
					ImGui::TableSetupColumn("ID##Mesh", ImGuiTableColumnFlags_WidthFixed);
				    ImGui::TableSetupColumn("Name##Mesh", ImGuiTableColumnFlags_WidthStretch);
				    ImGui::TableSetupColumn("Tracked##Mesh", ImGuiTableColumnFlags_WidthFixed);
				    ImGui::TableSetupColumn("Path##Mesh", ImGuiTableColumnFlags_WidthStretch);
				    ImGui::TableHeadersRow();
				    for (auto& mesh : ResourceManager::meshes)
				    {
				        ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%d", mesh.first);
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(mesh.second.name.c_str());
						ImGui::TableSetColumnIndex(2);
						ImGui::Text(mesh.second.isTracked ? "True" : "False");
						ImGui::TableSetColumnIndex(3);
						ImGui::Text(mesh.second.source.string().c_str());
				    }
				    ImGui::EndTable();
					ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Textures"))
                {
                    ImGui::InputText("Path##Tex", texBuff, MAX_ASSET_NAME_LENGTH);
					ImGui::SameLine();
					if(ImGui::Button("Import##Tex"))
					{
						std::string path = {texBuff};
						try
						{
							ResourceManager::addTexture(path);
						}
						catch (std::runtime_error&)
						{
							SDLFramework::showErrorMessage("Could not load texture", "Make sure the path is correct");
						}
					}
					ImGui::Separator();
					ImGui::BeginTable("Script##Tex", 4, flags);
					ImGui::TableSetupColumn("ID##Tex", ImGuiTableColumnFlags_WidthFixed);
				    ImGui::TableSetupColumn("Name##Tex", ImGuiTableColumnFlags_WidthStretch);
				    ImGui::TableSetupColumn("Tracked##Tex", ImGuiTableColumnFlags_WidthFixed);
				    ImGui::TableSetupColumn("Path##Tex", ImGuiTableColumnFlags_WidthStretch);
				    ImGui::TableHeadersRow();
				    for (auto& tex : ResourceManager::textures)
				    {
				        ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%d", tex.first);
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(tex.second.name.c_str());
						ImGui::TableSetColumnIndex(2);
						ImGui::Text(tex.second.isTracked ? "True" : "False");
						ImGui::TableSetColumnIndex(3);
						ImGui::Text(tex.second.source.string().c_str());
				    }
				    ImGui::EndTable();
					ImGui::EndTabItem();
                }
				if (ImGui::BeginTabItem("Scripts"))
                {
                    ImGui::InputText("Path##Scr", scrBuff, MAX_ASSET_NAME_LENGTH);
					ImGui::SameLine();
					if(ImGui::Button("Import##Scr"))
					{
						std::string path = {scrBuff};
						try
						{
							ResourceManager::addScript(path);
						}
						catch (std::runtime_error&)
						{
							SDLFramework::showErrorMessage("Could not load script", "Make sure the path is correct");
						}
					}
					ImGui::Separator();
					ImGui::BeginTable("Script##Script", 4, flags);
					ImGui::TableSetupColumn("ID##Script", ImGuiTableColumnFlags_WidthFixed);
				    ImGui::TableSetupColumn("Name##Script", ImGuiTableColumnFlags_WidthStretch);
				    ImGui::TableSetupColumn("Tracked##Script", ImGuiTableColumnFlags_WidthFixed);
				    ImGui::TableSetupColumn("Path##Script", ImGuiTableColumnFlags_WidthStretch);
				    ImGui::TableHeadersRow();
				    for (auto& scr : ResourceManager::scripts)
				    {
				        ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%d", scr.first);
						ImGui::TableSetColumnIndex(1);
						ImGui::Text(scr.second.name.c_str());
						ImGui::TableSetColumnIndex(2);
						ImGui::Text(scr.second.isTracked ? "True" : "False");
						ImGui::TableSetColumnIndex(3);
						ImGui::Text(scr.second.source.string().c_str());
				    }
				    ImGui::EndTable();
					ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
		ImGui::End();
	}

private:
	inline static char meshBuff[MAX_ASSET_NAME_LENGTH] = "";
	inline static char texBuff[MAX_ASSET_NAME_LENGTH] = "";
	inline static char scrBuff[MAX_ASSET_NAME_LENGTH] = "";
};

