#pragma once
#include "../GMesh.hpp"

class VKMesh : public GMesh
{
public:
	explicit VKMesh(const std::string& filepath);
	~VKMesh() override;

	void render() override;
};

