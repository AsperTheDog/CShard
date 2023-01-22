#include "Model.hpp"

#include "../../device/graphics/GFramework.hpp"
#include "../../Engine.hpp"

#include "../../device/graphics/GTexture.hpp"
#include "../../device/graphics/GMesh.hpp"

#include <gtx/transform.hpp>

Model::Model()
	: meshID(0), textureID(0), data({})
{

}

Model::~Model()
{
}

void Model::render()
{
	GFramework::get()->loadModelUniforms(this);

	if (Engine::isValidTexture(textureID))
		Engine::getTexture(textureID)->useTexture();
	else
		GFramework::get()->setDefaultTexture();

	if (Engine::isValidMesh(meshID))
		Engine::getMesh(meshID)->render(cullFace);
}

void Model::changePosition(glm::vec3 pos)
{
	this->data.pos = pos;
	this->data.changed = true;
}

void Model::changeRotation(glm::vec3 rot)
{
	this->data.rot = rot;
	this->data.changed = true;
}

void Model::changeScale(glm::vec3 scale)
{
	this->data.scale = scale;
	this->data.changed = true;
}

void Model::changeMesh()
{
	this->meshID = tempMeshID;
}

void Model::changeTexture()
{
	this->textureID = tempTexID;
}

void Model::calculateMatrix(PhysicalData& pData)
{
	if (!pData.changed && !data.changed) return;

	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pData.pos);
	modelMatrix = glm::translate(modelMatrix, data.pos);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(pData.rot.x), {1, 0, 0});
	modelMatrix = glm::rotate(modelMatrix, glm::radians(pData.rot.y), {0, 1, 0});
	modelMatrix = glm::rotate(modelMatrix, glm::radians(pData.rot.z), {0, 0, 1});
	modelMatrix = glm::rotate(modelMatrix, glm::radians(data.rot.x), {1, 0, 0});
	modelMatrix = glm::rotate(modelMatrix, glm::radians(data.rot.y), {0, 1, 0});
	modelMatrix = glm::rotate(modelMatrix, glm::radians(data.rot.z), {0, 0, 1});
	modelMatrix = glm::scale(modelMatrix, pData.scale);
	modelMatrix = glm::scale(modelMatrix, data.scale);

	data.changed = false;
}
