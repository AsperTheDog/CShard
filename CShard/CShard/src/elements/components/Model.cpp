#include "Model.hpp"

#include "../../Engine.hpp"
#include "../../ResourceManager.hpp"

#include <gtx/transform.hpp>
#include "../../device/graphics/opengl/OGLFramework.hpp"

Model::Model()
	: meshID(0), textureID(0), mat{0, 1}
{
}

void Model::render(PhysicalData& pData, bool material)
{
	OGLFramework::loadModelUniforms(*this, pData, material);

	if (ResourceManager::isValidTexture(textureID))
		ResourceManager::getTexture(textureID)->useTexture();
	else
		OGLFramework::setDefaultTexture();

	if (ResourceManager::isValidMesh(meshID))
		ResourceManager::getMesh(meshID)->render(cullFace);
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

glm::mat4 Model::getModelMatrix(PhysicalData& pData)
{
	if (pData.changed || data.changed) calculateMatrix(pData);
	return modelMatrix;
}

glm::mat4 Model::getInvModelMatrix(PhysicalData& pData)
{
	if (pData.changed || data.changed) calculateMatrix(pData);
	return invModelMatrix;
}

void Model::calculateMatrix(PhysicalData& pData)
{
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, pData.pos + data.pos);
	glm::vec3 finalRot = pData.rot + data.rot;
	modelMatrix = glm::rotate(modelMatrix, glm::radians(finalRot.x), {1, 0, 0});
	modelMatrix = glm::rotate(modelMatrix, glm::radians(finalRot.y), {0, 1, 0});
	modelMatrix = glm::rotate(modelMatrix, glm::radians(finalRot.z), {0, 0, 1});
	modelMatrix = glm::scale(modelMatrix, pData.scale);
	modelMatrix = glm::scale(modelMatrix, data.scale);
	invModelMatrix = glm::transpose(glm::inverse(modelMatrix));

	data.changed = false;
}

void Model::serialize(std::ofstream& wf)
{
	wf.write((char*) &meshID, sizeof(meshID));
	wf.write((char*) &textureID, sizeof(textureID));
	wf.write((char*) &data, sizeof(data));
	wf.write((char*) &mat, sizeof(mat));
	wf.write((char*) &cullFace, sizeof(cullFace));
}

void Model::deserialize(std::ifstream& wf)
{
	wf.read((char*) &meshID, sizeof(meshID));
	wf.read((char*) &textureID, sizeof(textureID));
	wf.read((char*) &data, sizeof(data));
	wf.read((char*) &mat, sizeof(mat));
	wf.read((char*) &cullFace, sizeof(cullFace));
	tempMeshID = meshID;
	tempTexID = textureID;
	data.changed = true;
}
