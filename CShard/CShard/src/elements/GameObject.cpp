#include "GameObject.hpp"

#include <fstream>
#include <iostream>
#include <ranges>

#include "../device/graphics/GFramework.hpp"
#include "../engine/ResourceManager.hpp"
#include "../engine/lua/Manager.hpp"
#include "components/Collider.hpp"

ComponentUnion::ComponentUnion()
{
	scr = Script();
}

Component::Component() : type(COMPONENT_SCRIPT)
{
}

Component::Component(ComponentType type) : type(type)
{
}

void Component::serialize(std::ofstream& wf)
{
	wf.write((char*) &type, sizeof(type));
	switch (type)
	{
	case COMPONENT_CAMERA: 
		value.cam.serialize(wf);
		break;
	case COMPONENT_COLLIDER: 
		value.coll.serialize(wf);
		break;
	case COMPONENT_MODEL: 
		value.mod.serialize(wf);
		break;
	case COMPONENT_SCRIPT: 
		value.scr.serialize(wf);
		break;
	case COMPONENT_LIGHT: 
		value.li.serialize(wf);
		break;
	case COMPONENT_BACKGROUND: 
		value.back.serialize(wf);
		break;
	}
}

void Component::deserialize(std::ifstream& wf)
{
	wf.read((char*) &type, sizeof(type));
	switch (type)
	{
	case COMPONENT_CAMERA: 
		value.cam.deserialize(wf);
		break;
	case COMPONENT_COLLIDER: 
		value.coll.deserialize(wf);
		break;
	case COMPONENT_MODEL: 
		value.mod.deserialize(wf);
		break;
	case COMPONENT_SCRIPT: 
		value.scr.deserialize(wf);
		break;
	case COMPONENT_LIGHT: 
		value.li.deserialize(wf);
		break;
	case COMPONENT_BACKGROUND: 
		value.back.deserialize(wf);
		break;
	}
}

GameObject::GameObject(const std::string& name)
{
	strcpy_s(this->name, sizeof(this->name), name.c_str());
}

void GameObject::addComponent(Component& comp)
{
	switch (comp.type)
	{
	case COMPONENT_CAMERA:
		comp.value.cam = Camera();
		break;
	case COMPONENT_COLLIDER: 
		comp.value.coll = Collider();
		ColliderStructure::add(this, &comp);
		break;
	case COMPONENT_MODEL: 
		comp.value.mod = Model();
		break;
	case COMPONENT_SCRIPT: 
		comp.value.scr = Script();
		break;
	case COMPONENT_LIGHT:
		comp.value.li = Light();
		lightCount++;
		GFramework::lightSourceCount++;
		break;
	case COMPONENT_BACKGROUND: 
		comp.value.back = Background();
		hasBackground = true;
		break;
	}
	this->components.emplace(compIdTracker, comp);
	compIdTracker++;
}

Component* GameObject::getComponent(uint32_t id)
{
	if (!components.contains(id)) return nullptr;
	return &components.at(id);
}

void GameObject::insertComponent(Component& comp)
{
	if (comp.type == COMPONENT_LIGHT)
	{
		lightCount++;
		GFramework::lightSourceCount++;
	}
	else if (comp.type == COMPONENT_BACKGROUND) 
		hasBackground = true;
	else if (comp.type == COMPONENT_COLLIDER)
		ColliderStructure::add(this, &comp);
	this->components.emplace(compIdTracker, comp);
	compIdTracker++;
}

void GameObject::insertComponent(uint32_t id, Component& comp)
{
	if (comp.type == COMPONENT_LIGHT)
	{
		lightCount++;
		GFramework::lightSourceCount++;
	}
	else if (comp.type == COMPONENT_BACKGROUND) 
		hasBackground = true;
	else if (comp.type == COMPONENT_COLLIDER)
		ColliderStructure::remove(this, &comp);

	this->components.emplace(id, comp);
}

void GameObject::removeComponent(uint32_t id)
{
	if (!components.contains(id)) return;
	components.erase(id);
}

void GameObject::processCollision()
{
	ColliderStructure::test();
}

void GameObject::processScripts(uint32_t objID, ScriptType type)
{
	for (auto& scr : components | std::views::values)
	{
		if (scr.type == COMPONENT_SCRIPT 
			&& scr.value.scr.type == type 
			&& ResourceManager::isValidScript(scr.value.scr.scriptID))
		{
			CSLua::Manager::runObjScript(objID, scr.value.scr.scriptID);
		}
	}
}

void GameObject::processBackground()
{
	if (!show || !hasBackground) return;
	GFramework::prepareShader(SHADER_BACK);
	components[0].value.back.render(this->modelData);
}

void GameObject::processLights()
{
	if (!show || lightCount == 0) return;
	GFramework::prepareShader(SHADER_BASE);

	for (auto& comp : components | std::views::values)
	{
		if (comp.type != COMPONENT_LIGHT) continue;
	
		GFramework::loadLightUniforms(comp.value.li, this->modelData);
	}
}

void GameObject::processModels(Camera& cam)
{
	if (!show) return;
	GFramework::prepareShader(SHADER_BASE);
	for (auto& comp : components | std::views::values) 
	{
		if (comp.type != COMPONENT_MODEL) continue;
		
		comp.value.mod.render(modelData, true, cam);
	}

	modelData.changed = false;
}

void GameObject::changePosition(glm::vec3 pos)
{
	this->modelData.pos = pos;
	this->modelData.changed = true;
}

void GameObject::changeRotation(glm::vec3 rot)
{
	this->modelData.rot = rot;
	this->modelData.changed = true;
}

void GameObject::changeScale(glm::vec3 scale)
{
	this->modelData.scale = scale;
	this->modelData.changed = true;
}

void GameObject::toggleActive()
{
	show = !show;
	GFramework::lightSourceCount += (show ? 1 : -1) * lightCount;
}
