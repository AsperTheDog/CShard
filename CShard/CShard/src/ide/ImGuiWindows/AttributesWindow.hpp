#pragma once
#include "ObjectWindow.hpp"

#include "../../engine/ResourceManager.hpp"
#include "../../device/graphics/GFramework.hpp"

#include "../../elements/components/Camera.hpp"
#include "../../elements/components/Model.hpp"
#include "../../elements/components/Script.hpp"
#include "../../elements/components/Background.hpp"
#include "../../elements/components/Light.hpp"

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
		GameObject* obj = ResourceManager::getObject(ObjectWindow::selectedObject);
		if (!obj) {
			ImGui::End();
			return;
		}
		ImGui::InputText(("Object name##" + std::to_string(ObjectWindow::selectedObject)).c_str(), obj->name, MAX_PATH_NAME_LENGTH);
		glm::vec3 pos{obj->modelData.pos};
		glm::vec3 rot{obj->modelData.rot};
		glm::vec3 scale{obj->modelData.scale};
		ImGui::DragFloat3("Position##obj", &pos.x, 0.1f, 10000.f, 10000.f);
		if (pos != obj->modelData.pos) obj->changePosition(pos);
		ImGui::DragFloat3("scale##obj", &scale.x, 0.1f, 0.f, 1000.f);
		if (scale != obj->modelData.scale) obj->changeScale(scale);
		ImGui::DragFloat3("Rotation##obj", &rot.x, 0.1f, 0.f, 360.f);
		if (rot != obj->modelData.rot) obj->changeRotation(rot);
		ImGui::Separator();
		ImGui::BeginDisabled(obj->hasBackground);
		ImGui::Combo("Input type", &tempCompype, componentNames, !obj->components.empty() ? 5 : 6);
		ImGui::SameLine();
		ImGui::EndDisabled();
		ImGui::BeginDisabled(obj->hasBackground || (GFramework::lightSourceCount == MAX_LIGHTS && compTypes[tempCompype] == COMPONENT_LIGHT));
		if (ImGui::Button("Create"))
		{
			Component comp = Component(compTypes[tempCompype]);
			obj->addComponent(comp);
		}
		if (GFramework::lightSourceCount == MAX_LIGHTS) ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,40,40,255));
		ImGui::Text("Lights left: %d", MAX_LIGHTS - GFramework::lightSourceCount);
		if (GFramework::lightSourceCount == MAX_LIGHTS) ImGui::PopStyleColor();
		ImGui::EndDisabled();
		ImGui::Separator();
		ImGui::BeginDisabled(!ResourceManager::selectedComponent.second);
		if (ImGui::Button("Clear selection"))
		{
			ResourceManager::selectedComponent.second = nullptr;
		}
		ImGui::EndDisabled();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Separator();
		uint32_t setToRemove = 0;
		bool doRemove = false;
		for (auto& comp : obj->components)
		{
			std::string uniqueID = std::to_string(comp.first);
			if (ImGui::Button(("-##Comp" + uniqueID).c_str()))
			{
				setToRemove = comp.first;
				doRemove = true;
			}
			ImGui::SameLine();
			bool isSelected = ResourceManager::selectedComponent.second == &comp.second;
			switch(comp.second.type)
			{
			case COMPONENT_CAMERA:
				if (ImGui::Selectable((uniqueID + ": Camera Component").c_str(), isSelected))
					ResourceManager::selectedComponent.second = &comp.second;
				showCamera(comp.second.value.cam, uniqueID);
				break;
			case COMPONENT_COLLIDER:
				if (ImGui::Selectable((uniqueID + ": Collider Component").c_str(), isSelected))
					ResourceManager::selectedComponent.second = &comp.second;
				showCollider(comp.second.value.coll, uniqueID);
				break;
			case COMPONENT_MODEL:
				if (ImGui::Selectable((uniqueID + ": Model Component").c_str(), isSelected))
					ResourceManager::selectedComponent.second = &comp.second;
				showModel(comp.second.value.mod, uniqueID);
				break;
			case COMPONENT_SCRIPT:
				if (ImGui::Selectable((uniqueID + ": Script Component").c_str(), isSelected))
					ResourceManager::selectedComponent.second = &comp.second;
				showScript(comp.second.value.scr, uniqueID);
				break;
			case COMPONENT_BACKGROUND:
				if (ImGui::Selectable((uniqueID + ": Background Component").c_str(), isSelected))
					ResourceManager::selectedComponent.second = &comp.second;
				showBackground(comp.second.value.back, uniqueID);
				break;
			case COMPONENT_LIGHT:
				if (ImGui::Selectable((uniqueID + ": LightSource Component").c_str(), isSelected))
					ResourceManager::selectedComponent.second = &comp.second;
				showLightSource(comp.second.value.li, uniqueID);
				break;
			}
			ImGui::Separator();
			ImGui::Separator();
		}
		if (doRemove)
		{
			obj->removeComponent(setToRemove);
		}

		if (ImGui::IsWindowFocused() && ImGuiManager::copied && ResourceManager::selectedComponent.second != nullptr)
		{
			copiedAttr = ResourceManager::selectedComponent.second;
			ObjectWindow::copiedObj = -1;
		}
		if (ImGui::IsWindowFocused() && ImGuiManager::pasted && copiedAttr != nullptr)
		{
			obj->insertComponent(*copiedAttr);
		}
		ImGui::End();
	}

private:
	static void showCamera(Camera& cam, std::string& uniqueID)
	{
		glm::vec3 tempVec = cam.pos;
		ImGui::InputFloat3(("Position##cam" + uniqueID).c_str(), &tempVec.x);
		if (tempVec != cam.pos) 
			cam.move(tempVec);

		tempVec = cam.dir;
		ImGui::InputFloat3(("Direction##cam" + uniqueID).c_str(), &tempVec.x);
		if (tempVec != cam.dir) 
			cam.lookAt(tempVec);
		ImGui::SameLine();
		if (ImGui::Button(("Normalize##cam" + uniqueID).c_str())) 
			cam.lookAt(glm::normalize(cam.dir));

		tempVec = cam.worldUp;
		ImGui::InputFloat3(("WorldUp##cam" + uniqueID).c_str(), &tempVec.x);
		if (tempVec != cam.worldUp)
			cam.changeWorldUp(tempVec);
		ImGui::SameLine();
		if (ImGui::Button(("Normalize##cam" + uniqueID).c_str())) 
			cam.changeWorldUp(glm::normalize(cam.worldUp));

		ImGui::Separator();
		float tempFOV = cam.FOV, tempNP = cam.nearPlane, tempFP = cam.farPlane;
		ImGui::DragFloat(("FOVy##cam" + uniqueID).c_str(), &tempFOV, 1.f, 1, 179);
		ImGui::DragFloat(("Near Plane##cam" + uniqueID).c_str(), &tempNP, 0.01f, 0.01f, 1);
		ImGui::DragFloat(("Far Plane##cam" + uniqueID).c_str(), &tempFP, 100.f, 10.f, 1000000.f);
		if (tempFOV != cam.FOV || tempNP != cam.nearPlane || tempFP != cam.farPlane)
			cam.changeLense(tempFOV, tempNP, tempFP);
	}

	static void showCollider(Collider& coll, std::string& uniqueID)
	{
		ImGui::InputFloat3(("Position##coll" + uniqueID).c_str(), &coll.position.x);
		ImGui::InputInt(("On enter script##coll" + uniqueID).c_str(), &coll.tempScripts[SCRIPT_COLLISION_ENTER], 1);
		coll.tempScripts[SCRIPT_COLLISION_ENTER] = std::max(0, coll.tempScripts[SCRIPT_COLLISION_ENTER]);
		ImGui::SameLine();
		if (ImGui::Button(("Reload##coll" + uniqueID).c_str()))
			coll.scripts[SCRIPT_COLLISION_ENTER] = coll.tempScripts[SCRIPT_COLLISION_ENTER];
		ImGui::InputInt(("On exit script##coll" + uniqueID).c_str(), &coll.tempScripts[SCRIPT_COLLISION_EXIT], 1);
		coll.tempScripts[SCRIPT_COLLISION_EXIT] = std::max(0, coll.tempScripts[SCRIPT_COLLISION_EXIT]);
		ImGui::SameLine();
		if (ImGui::Button(("Reload##coll" + uniqueID).c_str()))
			coll.scripts[SCRIPT_COLLISION_EXIT] = coll.tempScripts[SCRIPT_COLLISION_EXIT];
		ImGui::InputInt(("On inside script##coll" + uniqueID).c_str(), &coll.tempScripts[SCRIPT_COLLISION_IN], 1);
		coll.tempScripts[SCRIPT_COLLISION_IN] = std::max(0, coll.tempScripts[SCRIPT_COLLISION_IN]);
		ImGui::SameLine();
		if (ImGui::Button(("Reload##coll" + uniqueID).c_str()))
			coll.scripts[SCRIPT_COLLISION_IN] = coll.tempScripts[SCRIPT_COLLISION_IN];
		ImGui::InputInt(("On outside script##coll" + uniqueID).c_str(), &coll.tempScripts[SCRIPT_COLLISION_OUT], 1);
		coll.tempScripts[SCRIPT_COLLISION_OUT] = std::max(0, coll.tempScripts[SCRIPT_COLLISION_OUT]);
		ImGui::SameLine();
		if (ImGui::Button(("Reload##coll" + uniqueID).c_str()))
			coll.scripts[SCRIPT_COLLISION_OUT] = coll.tempScripts[SCRIPT_COLLISION_OUT];
		ImGui::Checkbox(("Static##coll" + uniqueID).c_str(), &coll.isStatic);
		ImGui::Checkbox(("Show##coll" + uniqueID).c_str(), &coll.draw);
	}

	static void showModel(Model& mod, std::string& uniqueID)
	{
		glm::vec3 tempVec = mod.data.pos;
		ImGui::DragFloat3(("Position##mod" + uniqueID).c_str(), &tempVec.x, 0.1f, 10000.f, 10000.f);
		if (tempVec != mod.data.pos) mod.changePosition(tempVec);

		tempVec = mod.data.scale;
		ImGui::DragFloat3(("scale##mod" + uniqueID).c_str(), &tempVec.x, 0.1f, 0.f, 1000.f);
		if (tempVec != mod.data.scale) mod.changeScale(tempVec);

		tempVec = mod.data.rot;
		ImGui::DragFloat3(("Rotation##mod" + uniqueID).c_str(), &tempVec.x, 1.f, -180.f, 180.f);
		if (tempVec != mod.data.rot) mod.changeRotation(tempVec);
		ImGui::Separator();
		ImGui::DragFloat(("Shininess##mod" + uniqueID).c_str(), &mod.mat.shininess, 0.2f, 1.f, 60.f);
		ImGui::DragFloat(("Emission##mod" + uniqueID).c_str(), &mod.mat.emission, 0.01f, 0, 1);
		ImGui::Separator();
		ImGui::InputInt(("Model##mod" + uniqueID).c_str(), &mod.tempMeshID, 1);
		mod.tempMeshID = std::max(0, mod.tempMeshID);
		ImGui::SameLine();
		if (ImGui::Button(("Reload##Meshmod" + uniqueID).c_str()))
			mod.changeMesh();
		ImGui::InputInt(("Texture##mod" + uniqueID).c_str(), &mod.tempTexID, 1);
		mod.tempTexID = std::max(0, mod.tempTexID);
		ImGui::SameLine();
		if (ImGui::Button(("Reload##Texmod" + uniqueID).c_str()))
			mod.changeTexture();

		ImGui::Checkbox(("Cull backface##mod" + uniqueID).c_str(), &mod.cullFace);
		//ImGui::Checkbox(("Cast shadows##mod" + uniqueID).c_str(), &mod.castShadows);
	}

	static void showScript(Script& scr, std::string& uniqueID)
	{
		ImGui::Combo(("Script type##scr" + uniqueID).c_str(), &scr.tempScrType, scriptTypes, 3);
		scr.type = scrTypes[scr.tempScrType];
		ImGui::InputInt(("Script##scr" + uniqueID).c_str(), &scr.tempScrID, 1);
		scr.tempScrID = std::max(0, scr.tempScrID);
		ImGui::SameLine();
		if (ImGui::Button(("Reload##scr" + uniqueID).c_str()))
			scr.setScript();
	}

	static void showBackground(Background& back, std::string& uniqueID)
	{
		ImGui::InputInt(("Texture##mod" + uniqueID).c_str(), &back.tempTexID, 1);
		back.tempTexID = std::max(0, back.tempTexID);
		ImGui::SameLine();
		if (ImGui::Button(("Reload##Texmod" + uniqueID).c_str()))
			back.setTexture();
	}

	static void showLightSource(Light& li, std::string& uniqueID)
	{
		ImGui::DragFloat3(("Position##li" + uniqueID).c_str(), &li.position.x, 0.1f, 10000.f, 10000.f);
		ImGui::ColorPicker3(("Color##li" + uniqueID).c_str(), &li.color.x);
		ImGui::Separator();
		ImGui::DragFloat(("Constant##li" + uniqueID).c_str(), &li.constant, 0.01f, 0, 1);
		ImGui::DragFloat(("Linear##li" + uniqueID).c_str(), &li.linear, 0.01f, 0, 1);
		ImGui::DragFloat(("Quadratic##li" + uniqueID).c_str(), &li.quadratic, 0.01f, 0, 1);
	}

	inline static int tempCompype = 0;
	inline static const char* componentNames[6] = {
		"Camera",
		"Collider",
		"Model",
		"Script",
		"Light",
		"Background"
	};
	inline static constexpr ComponentType compTypes[6] = {
		COMPONENT_CAMERA,
		COMPONENT_COLLIDER,
		COMPONENT_MODEL,
		COMPONENT_SCRIPT,
		COMPONENT_LIGHT,
		COMPONENT_BACKGROUND
	};
	inline static const char* scriptTypes[7] = {
		"On Start",
		"On Frame",
		"On End",
		"On Collision Enter",
		"On Collision In",
		"On Collision Exit",
		"On Collision Out"
	};
	inline static constexpr ScriptType scrTypes[7] = {
		SCRIPT_INIT,
		SCRIPT_FRAME,
		SCRIPT_DIE,
		SCRIPT_COLLISION_ENTER,
		SCRIPT_COLLISION_IN,
		SCRIPT_COLLISION_EXIT,
		SCRIPT_COLLISION_OUT
	};
	
	inline static Component* copiedAttr = 0;
};
