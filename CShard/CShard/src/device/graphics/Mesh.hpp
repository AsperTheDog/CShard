#pragma once
#include <cstdint>
#include <string>
#include <glm.hpp>
#include <vector>

#include "../../engine/AssetPath.hpp"

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
	explicit Mesh(const AssetPath& source);
	~Mesh();

	void commit(const AssetPath& source);
	void commit();
	void render(bool culling);

	void renderAsSelection();
	void rename(std::filesystem::path& newName);
	void reload();

	std::string name;
	std::filesystem::path source;
	bool isTracked = false;

private:
	void extractData(const std::filesystem::path& filename);

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