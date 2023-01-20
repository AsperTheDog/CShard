#pragma once
#include <string>

#include "../GTexture.hpp"

class OGLTexture final : public GTexture
{
public:
	explicit OGLTexture(std::string path);
	~OGLTexture() override;

	void useTexture() override;
	void renderAsBackground() override;
};

