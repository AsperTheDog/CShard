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

	void move(glm::vec3 pos);
	void lookAt(glm::vec3 dir);
	void changeWorldUp(glm::vec3 up);
	void changeLense(float FOV, float nearP = -1, float farP = -1);
	void updateAspectRatio(float aspectRatio);

	glm::vec3 pos{};
	glm::vec3 dir{};
	glm::vec3 worldUp{};

	float FOV{};
	float nearPlane{};
	float farPlane{};
	float aspectRatio{};
private:
	bool hasChangedView = false;
	bool hasChangedProj = false;
	glm::mat4 viewMatrix{};
	glm::mat4 projMatrix{};
};

