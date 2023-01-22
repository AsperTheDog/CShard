#pragma once
#include <string>

#include "../GTexture.hpp"

class OGLTexture final : public GTexture
{
public:
	explicit OGLTexture(const std::string& path);
	~OGLTexture() override;
	
	void useTexture() override;
	void renderAsBackground() override;

	uint32_t texture{};
};

class FBOGLTexture final : public FBTexture
{
public:
	FBOGLTexture(TexType type, uint32_t width, uint32_t height);
	~FBOGLTexture() override;

	void resize(uint32_t width, uint32_t height, char* data) override;

	uint32_t texture{};
};
