#pragma once
#include <vector>

#include "components/Camera.hpp"
#include "components/Collider.hpp"
#include "components/Model.hpp"
#include "components/Script.hpp"

#include <gtx/transform.hpp>

#include "components/Background.hpp"

class GameObject
{
public:
	void addCam();
	void addCollider();
	void addModel();
	void addScript();
	void addBackground();

	void processCollision();
	void processScript();
	void processRender();

	void changePosition(glm::vec3 pos);
	void changeRotation(glm::vec3 rot);
	void changeScale(glm::vec3 scale);

private:
	void calculateMatrix();

	glm::vec3 pos{};
	glm::vec3 rot{};
	glm::vec3 scale{};
	glm::mat4 modelMatrix{};

	std::vector<Camera> cams;
	std::vector<Collider> colls;
	std::vector<Model> models;
	std::vector<Script> scripts;
	std::vector<Background> backgrounds;
};


