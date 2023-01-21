#include "Camera.hpp"

#include "../../device/window/SDLFramework.hpp"

#include <gtc/matrix_transform.hpp>

Camera::Camera() : FOV(100.f), nearPlane(0.01f), farPlane(100000.f), aspectRatio(16/9.f)
{
	this->pos = glm::vec3(0);
	this->dir = glm::vec3(0, 0, -1);
	this->worldUp = glm::vec3(0, 1, 0);
}

glm::mat4 Camera::getViewMatrix()
{
	return this->viewMatrix;
}

void Camera::updateViewMatrix()
{
	this->viewMatrix = glm::lookAt(this->pos, this->pos + this->dir, this->worldUp);
}

glm::mat4 Camera::getProjMatrix()
{
	return this->projMatrix;
}

void Camera::updateProjMatrix()
{
	this->projMatrix = glm::perspective(FOV, SDLFramework::aspectRatio, nearPlane, farPlane);
}
