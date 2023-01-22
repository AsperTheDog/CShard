#pragma once
#include <imgui.h>

class GameOptionsWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		ImGui::Begin("Display options", isOpen, windowFlags);
		ImGui::InputFloat3("Position##Guicam", &Engine::activeCam->pos.x);
		ImGui::DragFloat3("Direction##Guicam", &Engine::activeCam->dir.x, 0.1f);
		ImGui::SameLine();
		if (ImGui::Button("Normalize##Guicam")) Engine::activeCam->dir = glm::normalize(Engine::activeCam->dir);
		ImGui::InputFloat3("WorldUp##Guicam", &Engine::activeCam->worldUp.x);
		ImGui::SameLine();
		if (ImGui::Button("Normalize##Guicam")) Engine::activeCam->worldUp = glm::normalize(Engine::activeCam->worldUp);
		ImGui::Separator();
		ImGui::DragFloat("FOVy##Guicam", &Engine::activeCam->FOV, 1.f, 1, 179);
		ImGui::DragFloat("Near Plane##Guicam", &Engine::activeCam->nearPlane, 0.01f, 0.01f, 1);
		ImGui::DragFloat("Far Plane##Guicam", &Engine::activeCam->farPlane, 100.f, 10.f, 1000000.f);
		Engine::activeCam->updateProjMatrix();
		Engine::activeCam->updateViewMatrix();
		ImGui::End();
	}
};

