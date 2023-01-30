#pragma once
#include "../../engine/ResourceManager.hpp"
#include "../ImGuiManager.hpp"

class ObjectWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGui::Begin("Objects", isOpen);
		if (ImGui::Button("Add object"))
		{
			ResourceManager::addObject();
		}
		ImGui::InputInt("##DeleteID", &tempDeleteID);
		ImGui::SameLine();
		if (ImGui::Button("Delete ID"))
		{
			ResourceManager::removeObject(tempDeleteID);
		}

		ImGui::Separator();
		int i = 0;
        for (auto& objElem : ResourceManager::sceneObjects)
        {
			std::string id = std::to_string(i);
			
			if (ImGui::Button(objElem.second.show ? ("Hide##" + id).c_str() : ("Show##" + id).c_str()))
			{
				objElem.second.toggleActive();
			}

			ImGui::SameLine();
			char buf[MAX_PATH_NAME_LENGTH + 6];
			sprintf_s(buf, "%d: %s", objElem.first, objElem.second.name);
			if (objElem.second.hasBackground) 
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(50,255,50,255));
			else if (objElem.second.lightCount != 0)
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,128,0,255));
            if (ImGui::Selectable(buf, (uint32_t)selectedObject == objElem.first))
            {
	            selectedObject = objElem.first;
			}
			if (objElem.second.hasBackground || objElem.second.lightCount != 0)
				ImGui::PopStyleColor();
			ImGui::Separator();
			++i;
        }
		if (ImGui::IsWindowFocused() && ImGuiManager::copied && selectedObject >= 0)
		{
			copiedObj = selectedObject;
		}
		if (ImGui::IsWindowFocused() && ImGuiManager::pasted && copiedObj >= 0)
		{
			ResourceManager::clone(copiedObj);
		}
		ImGui::End();
	}

	inline static int selectedObject = -1;
	inline static int copiedObj = -1;

private:
	inline static int tempDeleteID = 0;
};

