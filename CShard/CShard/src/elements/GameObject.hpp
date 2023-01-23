#pragma once
#include <string>
#include <vector>
#include "PhysicalData.hpp"

class Light;
class Background;
class Script;
class Model;
class Collider;
class Camera;

enum ComponentType
{
	COMPONENT_CAMERA,
	COMPONENT_COLLIDER,
	COMPONENT_MODEL,
	COMPONENT_SCRIPT,
	COMPONENT_LIGHT,
	COMPONENT_BACKGROUND
};

union ComponentUnion
{
	Camera* cam;
	Collider* coll;
	Model* mod;
	Script* scr;
	Background* back;
	Light* li;
};

struct Component
{
	ComponentType type;
	ComponentUnion value;
};

class GameObject
{
public:
	explicit GameObject(const std::string& name);

	void addComponent(Component& comp);
	std::vector<Component>::iterator removeComponent(std::vector<Component>::iterator it);

	void processCollision();
	void processScript();
	void processRender();

	void changePosition(glm::vec3 pos);
	void changeRotation(glm::vec3 rot);
	void changeScale(glm::vec3 scale);
	void processLights();

	char name[20]{};
	std::vector<Component> components;
	bool hasBackground = false;

	PhysicalData modelData;
private:
};


