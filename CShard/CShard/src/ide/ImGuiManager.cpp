#include "ImGuiManager.hpp"
#include "device/graphics/GFramework.hpp"

#include "imgui.h"

ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();

    ImGui::StyleColorsDark();

    GFramework::getInstance()->loadImGuiBackends();
}

ImGuiManager::~ImGuiManager()
{
    GFramework::getInstance()->destroyImGui();
}

void ImGuiManager::newFrame()
{
    GFramework::getInstance()->loadImGuiFrame();
}

void ImGuiManager::render()
{
    ImGui::Render();
}
