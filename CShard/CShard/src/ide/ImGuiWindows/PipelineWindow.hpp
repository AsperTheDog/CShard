#pragma once

#include "../../engine/Engine.hpp"

#include "imgui.h"

class PipelineWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		ImGui::Begin("Pipeline", isOpen, windowFlags);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(128, 128, 128, 255));
        ImGui::Selectable("Initialize frame");
        ImGui::PopStyleColor();
        ImGui::Separator();
		for (int n = 0; n < IM_ARRAYSIZE(Engine::pipelineOrder); n++)
        {
            auto item = Engine::pipelineOrder[n];
            ImGui::Selectable(names.at(item).c_str());

            if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
            {
                int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                if (n_next >= 0 && n_next < IM_ARRAYSIZE(Engine::pipelineOrder))
                {
                    Engine::pipelineOrder[n] = Engine::pipelineOrder[n_next];
                    Engine::pipelineOrder[n_next] = item;
                    ImGui::ResetMouseDragDelta();
                }
            }
        }
        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(128, 128, 128, 255));
        ImGui::Selectable("Render");
        ImGui::PopStyleColor();
		ImGui::End();
	}

private:
    inline static std::unordered_map<PipelineElement, std::string> names = {
        {PIPELINE_INPUT, "Input"},
        {PIPELINE_COLLISION, "Collision scripts"},
        {PIPELINE_SCRIPT, "Frame scripts"}
    };
};
