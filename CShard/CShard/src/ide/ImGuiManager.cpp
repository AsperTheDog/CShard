#include "ImGuiManager.hpp"

#include <gtx/transform.hpp>

#include "../device/window/SDLFramework.hpp"

#include "ImGuiWindows/AssetWindow.hpp"
#include "ImGuiWindows/AttributesWindow.hpp"
#include "ImGuiWindows/GameWindow.hpp"
#include "ImGuiWindows/InputWindow.hpp"
#include "ImGuiWindows/DiagnosticsWindow.hpp"
#include "ImGuiWindows/GameOptionsWindow.hpp"


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
	navigationCam = Camera();

	if (Engine::isIDE)
	{
		
	}
}

void ImGuiManager::newFrame()
{
    GFramework::get()->loadImGuiFrame();
}

void ImGuiManager::render()
{
    {
	    bool p_open = true;
	    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	   
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	        window_flags |= ImGuiWindowFlags_NoBackground;

	    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	    ImGui::Begin("DockSpace Demo", &p_open, window_flags);

    	ImGui::PopStyleVar();
	    ImGui::PopStyleVar(2);

	    // Submit the DockSpace
	    ImGuiIO& io = ImGui::GetIO();
	    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	    {
	        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	    }

		if (ImGui::BeginMenuBar())
	    {
	        if (ImGui::BeginMenu("File"))
	        {
	            //ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::MenuItem("Save project", "");
				ImGui::Separator();
				ImGui::MenuItem("New project", "");
				ImGui::MenuItem("Open project", "");
				ImGui::Separator();
				ImGui::MenuItem("Exit", "");
	            ImGui::EndMenu();
	        }
			if (ImGui::BeginMenu("Window"))
			{
				for (auto& windowCall : windowCalls)
				{
					if (ImGui::MenuItem(windowCall.name.c_str(), "", windowCall.isOpen))
						windowCall.isOpen = !windowCall.isOpen;
				}
				ImGui::EndMenu();
			}

	        ImGui::EndMenuBar();
	    }


	    for (auto& windowCall : windowCalls) windowCall.window(&windowCall.isOpen);

	    ImGui::End();
	}

    ImGui::Render();

    GFramework::get()->renderImgui();
	if (GameWindow::isHovering && SDLFramework::leftClick)
		updateSceneCam();
	GameWindow::isHovering = false;
	SDLFramework::leftClick = false;
}

void ImGuiManager::destroy()
{
    GFramework::get()->destroyImGui();
}

ImGuiIO* ImGuiManager::getIO()
{
    return io;
}

void ImGuiManager::addWindowCall(ImGuiWindowCall call, std::string name, bool defaultOpen)
{
	ImGuiManager::windowCalls.emplace_back(std::move(name), call, defaultOpen);
}

void ImGuiManager::updateSceneCam()
{
	int delta_x = SDLFramework::mousePos.x - SDLFramework::lastMousePos.x;
	int delta_y = SDLFramework::mousePos.y - SDLFramework::lastMousePos.y;
	float rotation_speed = 0.001f;
	glm::mat4 yaw = glm::rotate(rotation_speed * delta_x, navigationCam.worldUp);
	glm::mat4 pitch = glm::rotate(
		rotation_speed * delta_y, 
		glm::normalize(glm::cross(navigationCam.dir, navigationCam.worldUp)));
	navigationCam.lookAt(glm::vec3(pitch * yaw * glm::vec4(navigationCam.dir, 0.0f)));
	SDLFramework::lastMousePos.x = SDLFramework::mousePos.x;
	SDLFramework::lastMousePos.y = SDLFramework::mousePos.y;
}

void ImGuiManager::addImGuiWindows()
{
	ImGuiManager::addWindowCall(InputWindow::showWindow, "Input mappings", false);
	ImGuiManager::addWindowCall(ObjectWindow::showWindow, "Object list", true);
	ImGuiManager::addWindowCall(GameWindow::showWindow, "Game window", true);
	ImGuiManager::addWindowCall(AttributesWindow::showWindow, "Object attributes", true);
	ImGuiManager::addWindowCall(AssetWindow::showWindow, "Assets", true);
	ImGuiManager::addWindowCall(DiagnosticsWindow::showWindow, "Diagnostics", false);
	ImGuiManager::addWindowCall(GameOptionsWindow::showWindow, "Game control", false);
}
