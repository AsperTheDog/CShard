#pragma once
#include <glm.hpp>

#include "../PhysicalData.hpp"

class Model
{
public:
	explicit Model();
	~Model();

	void render();

	void changePosition(glm::vec3 pos);
	void changeRotation(glm::vec3 rot);
	void changeScale(glm::vec3 scale);

	void changeMesh();
	void changeTexture();

	uint32_t meshID;
	uint32_t textureID;

	int tempMeshID{}, tempTexID{};

	PhysicalData data;
private:
	void calculateMatrix(PhysicalData* pData);
	glm::mat4 modelMatrix{};
};

