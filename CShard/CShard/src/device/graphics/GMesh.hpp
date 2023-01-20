#pragma once

#include <glm.hpp>
#include <string>
#include <vector>

struct Vertex
{
	glm::vec3 coords;
	glm::vec3 normal;
	glm::vec2 texUV;
};

class GMesh
{
public:
	explicit GMesh(std::string filepath);
	virtual ~GMesh() = default;

	virtual void render() = 0;

protected:
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

