#pragma once
#include <fstream>
#include <glm.hpp>

#include "../PhysicalData.hpp"
#include "Camera.hpp"

struct Material
{
	float emission;
    float shininess;
};

class Model
{
public:
	explicit Model();

	void render(PhysicalData& pData, bool material, Camera& cam);

	void changePosition(glm::vec3 pos);
	void changeRotation(glm::vec3 rot);
	void changeScale(glm::vec3 scale);

	void changeMesh();
	void changeTexture();

	glm::mat4 getModelMatrix(PhysicalData& pData);
	glm::mat4 getInvModelMatrix(PhysicalData& pData);

	void calculateMatrix(PhysicalData& pData);

	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	int tempMeshID{}, tempTexID{};
	PhysicalData data;
	Material mat;
	bool cullFace = true;

private:
	uint32_t meshID;
	uint32_t textureID;
	glm::mat4 modelMatrix{};
	glm::mat4 invModelMatrix{};
};

