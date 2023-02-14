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

class Mesh final
{
public:
	Mesh() = default;
	explicit Mesh(const std::string& filepath);
	~Mesh();

	void commit(const std::string& filepath);
	void render(bool culling);

	void renderAsSelection();
	
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

struct PostVertex
{
	glm::vec2 pos;
};

class PostQuad final
{
public:
	static void init();
	static void render();

private:
	inline static uint32_t VAO{};
	inline static uint32_t VBO{};
};