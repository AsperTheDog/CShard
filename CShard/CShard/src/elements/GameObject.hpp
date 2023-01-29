#pragma once
#include <memory>
#include <string>
#include <vector>
#include "PhysicalData.hpp"

#include "../Config.hpp"

#include "components/Background.hpp"
#include "components/Camera.hpp"
#include "components/Collider.hpp"
#include "components/Light.hpp"
#include "components/Model.hpp"
#include "components/Script.hpp"

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
	Camera cam;
	Collider coll;
	Model mod;
	Script scr;
	Background back;
	Light li;

	ComponentUnion();
	~ComponentUnion(){}
};

struct Component
{
	ComponentType type;
	ComponentUnion value;

	Component();
	explicit Component(ComponentType type);
	~Component(){}
};

class GameObject
{
public:
	explicit GameObject(const std::string& name);

	void addComponent(Component& comp);
	void insertComponent(Component& comp);
	std::vector<Component>::iterator removeComponent(std::vector<Component>::iterator it);

	void processCollision();
	void processScript();
	void processBackground();
	void processRender();
	void processShadow();

	void changePosition(glm::vec3 pos);
	void changeRotation(glm::vec3 rot);
	void changeScale(glm::vec3 scale);
	void processLights();
	void toggleActive();

	char name[MAX_OBJ_NAME_LENGTH]{};
	std::vector<Component> components;
	uint32_t lightCount = 0;
	bool hasBackground = false;
	bool show = true;

	PhysicalData modelData;
private:
};


