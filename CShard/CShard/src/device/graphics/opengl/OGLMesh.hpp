#pragma once
#include <cstdint>
#include <string>
#include <glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 texCoords;
	glm::vec3 norm;
};

class OGLMesh final
{
public:
	OGLMesh() = default;
	explicit OGLMesh(const std::string& filepath);
	~OGLMesh();

	void commit(const std::string& filepath);
	void render(bool culling);
	
	std::string name;
private:
	void extractData(const std::string& filename);

	uint32_t VAO{};
	uint32_t VBO{};
	uint32_t EBO{};
	uint32_t indexNum = 0;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

//---------------------------------------------------

struct BackVertex
{
	glm::vec2 pos;
};

class OGLPostQuad final
{
public:
	explicit OGLPostQuad();
	~OGLPostQuad();

	void commit();
	void render();

private:
	uint32_t VAO{};
	uint32_t VBO{};
};