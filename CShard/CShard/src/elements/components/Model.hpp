#pragma once
#include <glm.hpp>

#include "../PhysicalData.hpp"

struct Material
{
	float emission;
    float shininess;
};

class Model
{
public:
	explicit Model();

	void render();

	void changePosition(glm::vec3 pos);
	void changeRotation(glm::vec3 rot);
	void changeScale(glm::vec3 scale);

	void changeMesh();
	void changeTexture();

	void calculateMatrix(PhysicalData& pData);

	uint32_t meshID;
	uint32_t textureID;
	int tempMeshID{}, tempTexID{};
	PhysicalData data;
	Material mat;
	glm::mat4 modelMatrix{};
	glm::mat4 invModelMatrix{};
	bool cullFace = true;
private:
};

