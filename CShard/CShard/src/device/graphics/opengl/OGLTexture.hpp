#pragma once
#include <string>

#include "../GTexture.hpp"
#include "../GraphicsHeader.hpp"

enum TextureOptions
{
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
	MIPLINEAR = GL_LINEAR_MIPMAP_LINEAR,
	CLAMP = GL_CLAMP_TO_EDGE,
	REPEAT = GL_REPEAT,
	MREPEAT = GL_MIRRORED_REPEAT
};

class OGLTexture final : public GTexture
{
public:
	explicit OGLTexture(
		const std::string& path, 
		TextureOptions min = MIPLINEAR,
		TextureOptions mag = LINEAR, 
		TextureOptions wrapS = REPEAT, 
		TextureOptions wrapT = MREPEAT);
	~OGLTexture() override;
	
	void useTexture() override;
	void renderAsBackground() override;

	uint32_t texture{};
};

class OGLEmptyTexture final : public GEmptyTexture
{
public:
	OGLEmptyTexture(
		TexType type, uint32_t width, uint32_t height, 
		TextureOptions min = LINEAR,
		TextureOptions mag = LINEAR, 
		TextureOptions wrapS = REPEAT, 
		TextureOptions wrapT = REPEAT);
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
