#include "Camera.hpp"

#include <iostream>

#include "../../device/window/SDLFramework.hpp"
#include "../../ide/ImGuiManager.hpp"

#include <gtx/transform.hpp>

Camera::Camera()
{
	this->move(glm::vec3(0));
	this->lookAt(glm::vec3(0, 0, -1));
	this->changeWorldUp(glm::vec3(0, 1, 0));

	this->changeLense(100.f, 0.1f, 100000.f);
}

glm::mat4 Camera::getViewMatrix()
{
	if (hasChangedView) this->updateViewMatrix();
	hasChangedView = false;
	return this->viewMatrix;
}

void Camera::updateViewMatrix()
{
	this->viewMatrix = glm::lookAt(this->pos, this->pos + this->dir, this->worldUp);
}

glm::mat4 Camera::getProjMatrix()
{
	if (hasChangedProj) this->updateProjMatrix();
	hasChangedProj = false;
	return this->projMatrix;
}

void Camera::updateProjMatrix()
{
	this->projMatrix = glm::perspective(glm::radians(FOV), aspectRatio, nearPlane, farPlane);
}

void Camera::move(glm::vec3 pos)
{
	this->pos = pos;
	hasChangedView = true;
}

void Camera::lookAt(glm::vec3 dir)
{
	this->dir = dir;
	this->right = glm::cross(dir, worldUp);
	hasChangedView = true;
}

void Camera::changeWorldUp(glm::vec3 worldUp)
{
	this->worldUp = worldUp;
	this->right = glm::cross(dir, worldUp);
	hasChangedView = true;
}

void Camera::changeLense(float FOV, float nearP, float farP)
{
	this->FOV = FOV;
	if (nearP > 0) nearPlane = nearP;
	if (farP > 0) farPlane = farP;
	hasChangedProj = true;
}

void Camera::updateAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	hasChangedProj = true;
}
