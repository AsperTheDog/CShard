#pragma once

#include <map>
#include <string>
#include "PhysicalData.hpp"

#include "../engine/Config.hpp"

#include "components/Background.hpp"
#include "components/Camera.hpp"
#include "components/Collider.hpp"
#include "components/Light.hpp"
#include "components/Model.hpp"
#include "components/Script.hpp"

class GameObject;

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

	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);
	void renderWireframe(PhysicalData& pData, Camera& cam);
};

class GameObject
{
public:
	explicit GameObject(const std::string& name);

	void addComponent(Component& comp);
	Component* getComponent(uint32_t id);
	void insertComponent(Component& comp);
	void insertComponent(uint32_t id, Component& comp);
	void removeComponent(uint32_t id);
	
	void processScripts(uint32_t objID, ScriptType type);
	void processBackground();
	void processModels(Camera& cam);

	void changePosition(glm::vec3 pos);
	void changeRotation(glm::vec3 rot);
	void changeScale(glm::vec3 scale);
	void processLights();
	void toggleActive();
	void processColliders(Camera& cam);

	char name[MAX_PATH_NAME_LENGTH]{};
	std::map<uint32_t, Component> components{};
	uint32_t lightCount = 0;
	bool hasBackground = false;
	bool show = true;

	PhysicalData modelData;
private:
	uint32_t compIdTracker = 0;
};


