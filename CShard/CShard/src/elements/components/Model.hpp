#pragma once
#include <string>

#include "../../device/graphics/GTexture.hpp"
#include "../../device/graphics/GMesh.hpp"

class Model
{
public:
	explicit Model();
	~Model();

	void render(glm::mat4 parentMat);

	void changePosition(glm::vec3 pos);
	void changeRotation(glm::vec3 rot);
	void changeScale(glm::vec3 scale);

	void changeMesh(std::string filepath);
	void changeTexture(std::string filepath);

private:
	void calculateMatrix();

	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale{};
	glm::mat4 modelMatrix{};
	GMesh* mesh;
	GTexture* texture;
};

