#include "GameObject.hpp"

#include "components/Camera.hpp"
#include "components/Collider.hpp"
#include "components/Model.hpp"
#include "components/Script.hpp"
#include "components/Background.hpp"

GameObject::GameObject(const std::string& name)
{
	sprintf(this->name, "%s", name.c_str());
}

void GameObject::addComponent(Component& comp)
{
	switch (comp.type)
	{
	case COMPONENT_CAMERA:
		comp.value.cam = new Camera();
		break;
	case COMPONENT_COLLIDER: 
		comp.value.coll = new Collider();
		break;
	case COMPONENT_MODEL: 
		comp.value.mod = new Model();
		break;
	case COMPONENT_SCRIPT: 
		comp.value.scr = new Script();
		break;
	case COMPONENT_BACKGROUND: 
		comp.value.back = new Background();
		hasBackground = true;
		break;
	}
	this->components.push_back(comp);
}

std::vector<Component>::iterator GameObject::removeComponent(std::vector<Component>::iterator it)
{
	if (it == this->components.end()) return it;
	switch (it->type)
	{
	case COMPONENT_CAMERA:
		delete it->value.cam;
		break;
	case COMPONENT_COLLIDER: 
		delete it->value.coll;
		break;
	case COMPONENT_MODEL: 
		delete it->value.mod;
		break;
	case COMPONENT_SCRIPT: 
		delete it->value.scr;
		break;
	case COMPONENT_BACKGROUND: 
		delete it->value.back;
		hasBackground = false;
		break;
	}
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
