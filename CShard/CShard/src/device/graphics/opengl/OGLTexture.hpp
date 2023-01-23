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

class OGLEmptyTexture final : public GEmptyTexture
{
public:
	OGLEmptyTexture(TexType type, uint32_t width, uint32_t height);
	~OGLEmptyTexture() override;

	void resize(uint32_t width, uint32_t height, char* data) override;

	uint32_t texture{};
};

class OGLCubeTexture final : public GCubeTexture
{
public:
	explicit OGLCubeTexture(uint32_t width, uint32_t height);
	~OGLCubeTexture() override;

	uint32_t texture{};
};
