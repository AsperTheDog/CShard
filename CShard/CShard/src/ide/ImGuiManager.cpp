#include "ImGuiManager.hpp"

#include <iostream>

#include "../device/graphics/GFramework.hpp"

#include "imgui.h"

std::vector<std::pair<ImGuiWindowCall, bool>> ImGuiManager::windowCalls;
ImGuiIO* ImGuiManager::io;

void ImGuiManager::init()
{
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();

    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    GFramework::get()->loadImGuiBackends();
}

void ImGuiManager::newFrame()
{
    GFramework::get()->loadImGuiFrame();
}

void ImGuiManager::render()
{
    {
	    bool p_open = true;
	    static bool opt_fullscreen = true;
	    static bool opt_padding = false;
	    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	    if (opt_fullscreen)
	    {
	        const ImGuiViewport* viewport = ImGui::GetMainViewport();
	        ImGui::SetNextWindowPos(viewport->WorkPos);
	        ImGui::SetNextWindowSize(viewport->WorkSize);
	        ImGui::SetNextWindowViewport(viewport->ID);
	        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	    }
	    else
	    {
	        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	    }

	    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	        window_flags |= ImGuiWindowFlags_NoBackground;

	    if (!opt_padding)
	        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	    if (!opt_padding)
	        ImGui::PopStyleVar();

	    if (opt_fullscreen)
	        ImGui::PopStyleVar(2);

	    // Submit the DockSpace
	    ImGuiIO& io = ImGui::GetIO();
	    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	    {
	        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	    }

	    for (auto& windowCall : windowCalls) windowCall.first(&windowCall.second);

	    ImGui::End();
	}

    ImGui::Render();

    GFramework::get()->renderImgui();
}

void ImGuiManager::destroy()
{
    GFramework::get()->destroyImGui();
}

ImGuiIO* ImGuiManager::getIO()
{
    return io;
}

void ImGuiManager::addWindowCall(ImGuiWindowCall call)
{
    ImGuiManager::windowCalls.emplace_back(call, true);
}
