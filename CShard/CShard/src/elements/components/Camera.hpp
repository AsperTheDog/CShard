#pragma once

#include <glm.hpp>

class Camera
{
public:
	Camera();

	glm::mat4 getViewMatrix();
	void updateViewMatrix();
	glm::mat4 getProjMatrix();
	void updateProjMatrix();

	glm::vec3 pos{};
	glm::vec3 dir{};
	glm::vec3 worldUp{};

	float FOV;
	float nearPlane;
	float farPlane;
	float aspectRatio;
private:
	glm::mat4 viewMatrix{};
	glm::mat4 projMatrix{};
};

