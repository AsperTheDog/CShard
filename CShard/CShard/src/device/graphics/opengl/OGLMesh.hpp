#pragma once

#include "../GMesh.hpp"

class OGLMesh final : public GMesh
{
public:
	explicit OGLMesh(const std::string& filepath);
	~OGLMesh() override;

	void render(bool culling) override;

	uint32_t VAO{};
	uint32_t VBO{};
	uint32_t EBO{};

	uint32_t indexNum = 0;
};

class BackOGLMesh final : public BackGMesh
{
public:
	explicit BackOGLMesh();
	~BackOGLMesh() override;

	void render() override;

private:
	uint32_t VAO{};
	uint32_t VBO{};
};