#include "GameObject.hpp"

#include <iostream>

#include "../device/graphics/GFramework.hpp"

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
		break;
	case COMPONENT_BACKGROUND: 
		comp.value.back = Background();
		hasBackground = true;
		break;
	}
	GFramework::lightSourceCount += lightCount;
	this->components.push_back(comp);
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

	this->components.push_back(comp);
}

std::vector<Component>::iterator GameObject::removeComponent(std::vector<Component>::iterator it)
{
	if (it == this->components.end()) return it;
	if (it->type == COMPONENT_BACKGROUND) hasBackground = false;
	else if (it->type == COMPONENT_LIGHT) {
		lightCount--;
		GFramework::lightSourceCount--;
	}
	return this->components.erase(it);
}

void GameObject::processCollision()
{
	
}

void GameObject::processScript()
{

}

void GameObject::processLights()
{
	if (!show) return;
	for (auto& comp : components)
	{
		if (comp.type == COMPONENT_LIGHT)
			GFramework::get()->loadLightUniforms(comp.value.li, this->modelData);
	}
}

void GameObject::processRender()
{
	if (!show) return;
	for (auto& comp : components) 
	{
		if (comp.type == COMPONENT_BACKGROUND) comp.value.back.render();
		else if (comp.type == COMPONENT_MODEL)
		{
			comp.value.mod.calculateMatrix(modelData);
			comp.value.mod.render();
		}
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
