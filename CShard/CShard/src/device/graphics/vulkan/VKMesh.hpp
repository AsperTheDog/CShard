#pragma once
#include "../GMesh.hpp"

class VKMesh final : public GMesh
{
public:
	explicit VKMesh(const std::string& filepath);
	~VKMesh() override;

	void render() override;
};


class BackVKMesh final : public BackGMesh
{
public:
	explicit BackVKMesh();
	~BackVKMesh() override;

	void render() override;

private:
	uint32_t VAO{};
	uint32_t VBO{};
};