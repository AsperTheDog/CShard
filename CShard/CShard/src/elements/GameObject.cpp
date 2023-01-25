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
		GFramework::lightSourceCount++;
		break;
	case COMPONENT_BACKGROUND: 
		comp.value.back = Background();
		hasBackground = true;
		break;
	}
	this->components.push_back(comp);
}

std::vector<Component>::iterator GameObject::removeComponent(std::vector<Component>::iterator it)
{
	if (it == this->components.end()) return it;
	return this->components.erase(it);
}

void GameObject::processCollision()
{
	
}

void GameObject::processScript()
{

}

void GameObject::processRender()
{
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

void GameObject::processLights()
{
	for (auto& comp : components)
	{
		if (comp.type == COMPONENT_LIGHT)
			GFramework::get()->loadLightUniforms(comp.value.li, this->modelData);
	}
}
