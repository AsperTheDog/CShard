#include "GameObject.hpp"

void GameObject::addCam()
{
	this->cams.emplace_back();
}

void GameObject::addCollider()
{
	ColliderData data = {COLLIDE_SPHERE, };
	data.val.radius = 1.f;
	this->colls.emplace_back(data);
}

void GameObject::addModel()
{
	this->models.emplace_back();
}

void GameObject::addScript()
{
	this->scripts.emplace_back();
}

void GameObject::addBackground()
{
	this->backgrounds.emplace_back();
}

void GameObject::processCollision()
{
	for (auto& coll : colls) coll.testCollisions();
}

void GameObject::processScript()
{

}

void GameObject::processRender()
{
	for (auto& background : backgrounds) background.render();
	for (auto& model : models) model.render(modelMatrix);
}

void GameObject::changePosition(glm::vec3 pos)
{
	this->pos = pos;
	this->calculateMatrix();
}

void GameObject::changeRotation(glm::vec3 rot)
{
	this->rot = rot;
	this->calculateMatrix();
}

void GameObject::changeScale(glm::vec3 scale)
{
	this->scale = scale;
	this->calculateMatrix();
}


void GameObject::calculateMatrix()
{
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, rot.x, {1, 0, 0});
	modelMatrix = glm::rotate(modelMatrix, rot.y, {0, 1, 0});
	modelMatrix = glm::rotate(modelMatrix, rot.z, {0, 0, 1});
	modelMatrix = glm::scale(modelMatrix, scale);
}
