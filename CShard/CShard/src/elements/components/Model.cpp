#include "Model.hpp"

#include <utility>
#include "../../device/graphics/GFramework.hpp"

#include <gtx/transform.hpp>

Model::Model() : pos(0), rot(0), mesh(nullptr), texture(nullptr)
{
	this->calculateMatrix();
}

Model::~Model()
{
	delete this->mesh;
	delete this->texture;
}

void Model::render(glm::mat4 parentMat)
{
	if (this->texture)
		this->texture->useTexture();
	else
		GFramework::get()->setDefaultTexture();
	if (this->mesh)
		this->mesh->render();
}

void Model::changePosition(glm::vec3 pos)
{
	this->pos = pos;
	this->calculateMatrix();
}

void Model::changeRotation(glm::vec3 rot)
{
	this->rot = rot;
	this->calculateMatrix();
}

void Model::changeScale(glm::vec3 scale)
{
	this->scale = scale;
	this->calculateMatrix();
}

void Model::changeMesh(std::string filepath)
{
	delete this->mesh;
	this->mesh = GFramework::get()->createMesh(std::move(filepath));
}

void Model::changeTexture(std::string filepath)
{
	delete this->texture;
	this->texture = GFramework::get()->createTexture(std::move(filepath));
}

void Model::calculateMatrix()
{
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::rotate(modelMatrix, rot.x, {1, 0, 0});
	modelMatrix = glm::rotate(modelMatrix, rot.y, {0, 1, 0});
	modelMatrix = glm::rotate(modelMatrix, rot.z, {0, 0, 1});
	modelMatrix = glm::scale(modelMatrix, scale);
}
