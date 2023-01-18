#include "ImGuiManager.hpp"

#include <iostream>

#include "../device/graphics/GFramework.hpp"

#include "imgui.h"

ImGuiIO* ImGuiManager::io;
std::vector<ImGuiWindowCall> ImGuiManager::windowCalls;

void ImGuiManager::init()
{
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();

    ImGui::StyleColorsDark();

    GFramework::get()->loadImGuiBackends();
}

void ImGuiManager::newFrame()
{
    GFramework::get()->loadImGuiFrame();
}

void ImGuiManager::render()
{
    for (auto imguiCall : windowCalls) 
        imguiCall();
    ImGui::Render();

    GFramework::get()->renderImgui();
}

void ImGuiManager::destroy()
{
    GFramework::get()->destroyImGui();
}

void ImGuiManager::addWindowCall(ImGuiWindowCall call)
{
    ImGuiManager::windowCalls.push_back(call);
}
