#pragma once
#include "../../Engine.hpp"

class ObjectWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGui::Begin("Objects", isOpen);
		if (ImGui::Button("Add object"))
		{
			Engine::addObject();
		}
		ImGui::InputInt("##DeleteID", &tempDeleteID);
		ImGui::SameLine();
		if (ImGui::Button("Delete ID"))
		{
			Engine::removeObject(tempDeleteID);
		}

		ImGui::Separator();
        for (auto& objElem : Engine::sceneObjects)
        {
			char buf[32];
			sprintf(buf, "%d: %s", objElem.first, objElem.second.name);
			if (objElem.second.hasBackground) 
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0,255,0,255));
            if (ImGui::Selectable(buf, (uint32_t)selectedObject == objElem.first))
                selectedObject = objElem.first;
			if (objElem.second.hasBackground)
				ImGui::PopStyleColor();
			ImGui::Separator();
        }
		ImGui::End();
	}

	inline static int selectedObject = -1;

private:
	inline static int tempDeleteID = 0;
};

