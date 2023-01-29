#include "ImGuiManager.hpp"

#include <gtx/transform.hpp>

#include "../device/window/SDLFramework.hpp"

#include "ImGuiWindows/AssetWindow.hpp"
#include "ImGuiWindows/AttributesWindow.hpp"
#include "ImGuiWindows/GameWindow.hpp"
#include "ImGuiWindows/InputWindow.hpp"
#include "ImGuiWindows/DiagnosticsWindow.hpp"
#include "ImGuiWindows/GameOptionsWindow.hpp"
#include "ImGuiWindows/PostEffectWindow.hpp"


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
	keyDowns.emplace(SDLK_w, false);
	keyDowns.emplace(SDLK_a, false);
	keyDowns.emplace(SDLK_s, false);
	keyDowns.emplace(SDLK_d, false);
	keyDowns.emplace(SDLK_SPACE, false);
	keyDowns.emplace(SDLK_LSHIFT, false);
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
				if (ImGui::MenuItem("Save project", ""))
					showSaveWin = true;
				
				if (ImGui::MenuItem("Open project", ""))
					showLoadWin = true;

				if (ImGui::MenuItem("New project", ""))
					showNewWin = true;
				
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", ""))
					Engine::shouldQuit = true;
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

		
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		showSaveModal();

		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		showLoadModal();

		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		showCreateModal();		

	    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		showExitModal();

	    ImGui::End();
	}
    ImGui::Render();
    GFramework::get()->renderImgui();
}

void ImGuiManager::update()
{
	if (GameWindow::isFocused)
	{
		if (SDLFramework::leftClick)
			updateSceneCamDir();
		updateSceneCamPos();
	}
	SDLFramework::leftClick = false;
	ImGuiManager::copied = false;
	ImGuiManager::pasted = false;
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

void ImGuiManager::updateSceneCamDir()
{
	float delta_x = mousePos.x - lastMousePos.x;
	float delta_y = mousePos.y - lastMousePos.y;
	float rotation_speed = 0.001f;
	glm::mat4 yaw = glm::rotate(rotation_speed * delta_x, navigationCam.worldUp);
	glm::mat4 pitch = glm::rotate(
		rotation_speed * delta_y, 
		glm::normalize(glm::cross(navigationCam.dir, navigationCam.worldUp)));
	navigationCam.lookAt(glm::vec3(pitch * yaw * glm::vec4(navigationCam.dir, 0.0f)));
	lastMousePos.x = mousePos.x;
	lastMousePos.y = mousePos.y;
}

void ImGuiManager::updateSceneCamPos()
{
	glm::vec3 movement{0};
	if (keyDowns.at(SDLK_w)) movement += navigationCam.dir;
	if (keyDowns.at(SDLK_s)) movement -= navigationCam.dir;
	if (keyDowns.at(SDLK_d)) movement += navigationCam.right;
	if (keyDowns.at(SDLK_a)) movement -= navigationCam.right;
	if (keyDowns.at(SDLK_SPACE)) movement += navigationCam.worldUp;
	if (keyDowns.at(SDLK_LSHIFT)) movement -= navigationCam.worldUp;
	if (movement != glm::vec3(0))
		navigationCam.move(navigationCam.pos + glm::normalize(movement) * (Engine::dt * movementMult));
}

void ImGuiManager::showCreateModal()
{
	if (showNewWin) ImGui::OpenPopup("Save current");

    if (ImGui::BeginPopupModal("Save current", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Do you want to save the current project?");
        ImGui::Separator();
		ImGui::InputText("##currProjectName", nameBuff, MAX_OBJ_NAME_LENGTH);
		ImGui::BeginDisabled(strcmp(nameBuff, "") == 0);
        if (ImGui::Button("Save", ImVec2(90, 0)))
        {
			Engine::compileProject(nameBuff);
			Engine::resetProject();
			showNewWin = false;
            ImGui::CloseCurrentPopup();
        }
		ImGui::EndDisabled();
	    ImGui::SameLine();
		if (ImGui::Button("Don't save", ImVec2(90, 0)))
        {
			Engine::resetProject();
			showNewWin = false;
            ImGui::CloseCurrentPopup();
        }
	    ImGui::SetItemDefaultFocus();
	    ImGui::SameLine();
	    if (ImGui::Button("Cancel", ImVec2(90, 0)))
	    {
			showNewWin = false;
		    ImGui::CloseCurrentPopup();
	    }
	    ImGui::EndPopup();
    }
}

void ImGuiManager::showLoadModal()
{
	if (showLoadWin) ImGui::OpenPopup("Get name");

    if (ImGui::BeginPopupModal("Get name", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Insert the name of the project");
        ImGui::Separator();
		ImGui::InputText("##loadProjectName", nameBuff, MAX_OBJ_NAME_LENGTH);
		ImGui::BeginDisabled(strcmp(nameBuff, "") == 0);
        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
			Engine::loadProject(nameBuff);
			showLoadWin = false;
            ImGui::CloseCurrentPopup();
        }
		ImGui::EndDisabled();
	    ImGui::SetItemDefaultFocus();
	    ImGui::SameLine();
	    if (ImGui::Button("Cancel", ImVec2(120, 0)))
	    {
			showLoadWin = false;
		    ImGui::CloseCurrentPopup();
	    }
	    ImGui::EndPopup();
    }
}

void ImGuiManager::showSaveModal()
{
	if (showSaveWin) ImGui::OpenPopup("Add name");

    if (ImGui::BeginPopupModal("Add name", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Set a name for the project");
        ImGui::Separator();
		ImGui::InputText("##saveProjectName", nameBuff, MAX_OBJ_NAME_LENGTH);
		ImGui::BeginDisabled(strcmp(nameBuff, "") == 0);
        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
			Engine::compileProject(nameBuff);
			showSaveWin = false;
            ImGui::CloseCurrentPopup();
        }
		ImGui::EndDisabled();
	    ImGui::SetItemDefaultFocus();
	    ImGui::SameLine();
	    if (ImGui::Button("Cancel", ImVec2(120, 0)))
	    {
			showSaveWin = false;
		    ImGui::CloseCurrentPopup();
	    }
	    ImGui::EndPopup();
    }
}

void ImGuiManager::showExitModal()
{
	if (Engine::shouldQuit) ImGui::OpenPopup("Confirm quit");

	if (ImGui::BeginPopupModal("Confirm quit", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want to save before quitting?");
		ImGui::Separator();
		ImGui::InputText("##currProjectName", nameBuff, MAX_OBJ_NAME_LENGTH);
		ImGui::BeginDisabled(strcmp(nameBuff, "") == 0);
		if (ImGui::Button("Save", ImVec2(90, 0)))
		{
			Engine::compileProject(nameBuff);
			Engine::confirmQuit = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndDisabled();
		ImGui::SameLine();
		if (ImGui::Button("Don't save", ImVec2(90, 0)))
		{
			Engine::confirmQuit = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(90, 0)))
		{
			Engine::shouldQuit = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
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
	ImGuiManager::addWindowCall(PostEffectWindow::showWindow, "Post Effects", false);
}
