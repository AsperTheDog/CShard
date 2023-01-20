#pragma once
#include <algorithm>

#include "imgui.h"
#include "../../input/InputManager.hpp"


class InputWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		ImGui::Begin("Input Mapper", isOpen, windowFlags);
		ImGui::InputInt("ID", &tempID);
		tempID = std::max(0, tempID);
		ImGui::Combo("Input type", &tempType, InputManager::inputNames, 5);
		bool validValue = true;
		switch(tempType)
		{
		case 0:
		case 1:
			ImGui::InputText("Value", tempValue, 30);
			validValue = InputManager::keyboardKeys.contains(tempValue);
			break;
		case 2:
		case 3:
			ImGui::InputInt("Button", &tempButton);
			tempButton = std::clamp(tempButton, 0, 255);
			break;
		case 4:
			ImGui::Checkbox("Upwards", &tempWheel);
			break;
		default: 
			validValue = false;
		}

		bool isIDInUse = InputManager::inputMappings.contains(tempID);
		if (isIDInUse) ImGui::Text("That ID already exists!");
		if (!validValue) ImGui::Text("Invalid keyboard key");

		ImGui::BeginDisabled(isIDInUse || !validValue);
		if (ImGui::Button("Add new mapper"))
		{
			InputMapping mapping = {InputManager::inputTypes.at(tempType), {0}};
			switch(tempType)
			{
			case 0:
			case 1:
				mapping.val.key = InputManager::keyboardKeys.at(tempValue);
				break;
			case 2:
			case 3:
				mapping.val.mbutton = tempButton;
				break;
			case 4:
				mapping.val.wheelDir = tempWheel;
				break;
			default:
				mapping.val.key = 0;
			}
			InputManager::addMapping(tempID, mapping);
			tempID++;
		}
		ImGui::EndDisabled();

		ImGui::Separator();
		ImGui::InputInt("##DeleteID", &tempDeleteID);
		ImGui::SameLine();
		if (ImGui::Button("Delete ID"))
		{
			InputManager::removeMapping(tempDeleteID);
		}

		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
		ImGui::BeginTable("Mappings", 3, flags);
		ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
	    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch);
	    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
	    ImGui::TableHeadersRow();
	    for (auto& map : InputManager::inputMappings)
	    {
	        ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%d", map.first);
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(InputManager::inputTypeNames.at(map.second.type).c_str());
			ImGui::TableSetColumnIndex(2);
	        if (map.second.type == EVENT_KEYDOWN || map.second.type == EVENT_KEYUP)
				ImGui::Text(InputManager::keyboardKeyNames.at(map.second.val.key).c_str());
			else if (map.second.type == EVENT_MOUSEBUTTONDOWN || map.second.type == EVENT_MOUSEBUTTONUP)
				ImGui::Text("button %d", map.second.val.mbutton);
			else if (map.second.type == EVENT_MOUSEWHEEL)
				ImGui::Text(map.second.val.wheelDir ? "Up" : "Down");
	    }
	    ImGui::EndTable();
		ImGui::End();
	}

private:
	inline static int tempDeleteID = 0;
	inline static int tempID = 0;
	inline static int tempButton = 0;
	inline static bool tempWheel = false;
	inline static int tempType = 0;
	inline static char tempValue[20] = "";
};

