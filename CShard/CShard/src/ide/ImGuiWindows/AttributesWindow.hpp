#pragma once
#include "ObjectWindow.hpp"

#include "../../Engine.hpp"

#include "../../elements/components/Camera.hpp"
#include "../../elements/components/Collider.hpp"
#include "../../elements/components/Model.hpp"
#include "../../elements/components/Script.hpp"
#include "../../elements/components/Background.hpp"

class AttributesWindow
{
public:
	static void showWindow(bool* isOpen)
	{
		if (!*isOpen) return;
		ImGui::Begin("Attributes", isOpen);
		if (ObjectWindow::selectedObject < 0) {
			ImGui::End();
			return;
		}
		GameObject* obj = Engine::getObject(ObjectWindow::selectedObject);
		if (!obj) {
			ImGui::End();
			return;
		}
		ImGui::InputText("Object name", obj->name, 20);
		glm::vec3 pos{obj->modelData.pos};
		glm::vec3 rot{obj->modelData.rot};
		glm::vec3 scale{obj->modelData.scale};
		ImGui::DragFloat3("Position##obj", &pos.x, 1.f, 10000.f, 10000.f);
		if (pos != obj->modelData.pos) obj->changePosition(pos);
		ImGui::DragFloat3("scale##obj", &scale.x, 1.f, 0.f, 1000.f);
		if (scale != obj->modelData.scale) obj->changePosition(scale);
		ImGui::DragFloat3("Rotation##obj", &rot.x, 1.f, 0.f, 360.f);
		if (rot != obj->modelData.rot) obj->changePosition(rot);
		ImGui::Separator();
		ImGui::BeginDisabled(obj->hasBackground);
		ImGui::Combo("Input type", &tempType, componentNames, !obj->components.empty() ? 4 : 5);
		ImGui::SameLine();
		if (ImGui::Button("Create"))
		{
			Component comp = Component(compTypes[tempType]);
			obj->addComponent(comp);
		}
		ImGui::EndDisabled();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Separator();
		for (auto i = obj->components.begin(); i != obj->components.end(); ++i)
		{
			std::string uniqueID = std::to_string(i - obj->components.begin());
			if (ImGui::Button(("-##Comp" + uniqueID).c_str()))
			{
				i = obj->removeComponent(i);
				if (i == obj->components.end()) break;
			}
			switch(i->type)
			{
			case COMPONENT_CAMERA:
				ImGui::SameLine();
				ImGui::Text("Camera Component");
				showCamera(i->value.cam, uniqueID);
				break;
			case COMPONENT_COLLIDER:
				ImGui::SameLine();
				ImGui::Text("Collider Component");
				showCollider(i->value.coll, uniqueID);
				break;
			case COMPONENT_MODEL:
				ImGui::SameLine();
				ImGui::Text("Model Component");
				showModel(i->value.mod, uniqueID);
				break;
			case COMPONENT_SCRIPT:
				ImGui::SameLine();
				ImGui::Text("Script Component");
				showScript(i->value.scr, uniqueID);
				break;
			case COMPONENT_BACKGROUND:
				ImGui::SameLine();
				ImGui::Text("Background Component");
				showBackground(i->value.back, uniqueID);
				break;
			default: ;
			}
			ImGui::Separator();
			ImGui::Separator();
		}
		ImGui::End();
	}

private:
	static void showCamera(Camera* cam, std::string& uniqueID)
	{
		ImGui::InputFloat3(("Position##cam" + uniqueID).c_str(), &cam->pos.x);
		ImGui::InputFloat3(("Direction##cam" + uniqueID).c_str(), &cam->dir.x);
		ImGui::SameLine();
		if (ImGui::Button(("Normalize##cam" + uniqueID).c_str())) cam->dir = glm::normalize(cam->dir);
		ImGui::InputFloat3(("WorldUp##cam" + uniqueID).c_str(), &cam->worldUp.x);
		ImGui::SameLine();
		if (ImGui::Button(("Normalize##cam" + uniqueID).c_str())) cam->worldUp = glm::normalize(cam->worldUp);
		ImGui::Separator();
		ImGui::DragFloat(("FOVy##cam" + uniqueID).c_str(), &cam->FOV, 1.f, 1, 179);
		ImGui::DragFloat(("Near Plane##cam" + uniqueID).c_str(), &cam->nearPlane, 0.01f, 0.01f, 1);
		ImGui::DragFloat(("Far Plane##cam" + uniqueID).c_str(), &cam->farPlane, 100.f, 10.f, 1000000.f);
	}

	static void showCollider(Collider* coll, std::string& uniqueID)
	{
		
	}

	static void showModel(Model* mod, std::string& uniqueID)
	{
		glm::vec3 pos{mod->data.pos};
		glm::vec3 rot{mod->data.rot};
		glm::vec3 scale{mod->data.scale};
		ImGui::DragFloat3(("Position##mod" + uniqueID).c_str(), &pos.x, 1.f, 10000.f, 10000.f);
		if (pos != mod->data.pos) mod->changePosition(pos);
		ImGui::DragFloat3(("scale##mod" + uniqueID).c_str(), &scale.x, 1.f, 0.f, 1000.f);
		if (scale != mod->data.scale) mod->changeScale(scale);
		ImGui::DragFloat3(("Rotation##mod" + uniqueID).c_str(), &rot.x, 1.f, -180.f, 180.f);
		if (rot != mod->data.rot) mod->changeRotation(rot);
		ImGui::Separator();
		ImGui::InputInt(("Model##mod" + uniqueID).c_str(), &mod->tempMeshID, 1);
		mod->tempMeshID = std::max(0, mod->tempMeshID);
		ImGui::SameLine();
		if (ImGui::Button(("Reload##Meshmod" + uniqueID).c_str()))
		{
			mod->changeMesh();
		}
		ImGui::InputInt(("Texture##mod" + uniqueID).c_str(), &mod->tempTexID, 1);
		mod->tempTexID = std::max(0, mod->tempTexID);
		ImGui::SameLine();
		if (ImGui::Button(("Reload##Texmod" + uniqueID).c_str()))
		{
			mod->changeTexture();
		}
	}

	static void showScript(Script* scr, std::string& uniqueID)
	{
		
	}

	static void showBackground(Background* back, std::string& uniqueID)
	{
		ImGui::InputInt(("Texture##mod" + uniqueID).c_str(), &back->tempTexID, 1);
		back->tempTexID = std::max(0, back->tempTexID);
		ImGui::SameLine();
		if (ImGui::Button(("Reload##Texmod" + uniqueID).c_str()))
		{
			back->setTexture();
		}
	}

	inline static int tempType = 0;
	inline static const char* componentNames[5] = {
		"Camera",
		"Collider",
		"Model",
		"Script",
		"Background"
	};
	inline static constexpr ComponentType compTypes[5] = {
		COMPONENT_CAMERA,
		COMPONENT_COLLIDER,
		COMPONENT_MODEL,
		COMPONENT_SCRIPT,
		COMPONENT_BACKGROUND
	};
};
