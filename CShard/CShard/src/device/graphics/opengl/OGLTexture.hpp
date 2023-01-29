#pragma once
#include <string>

#include "../GraphicsHeader.hpp"

enum TexType
{
	COLOR,
	DEPTH
};

enum TextureOptions
{
	NEAREST = GL_NEAREST,
	LINEAR = GL_LINEAR,
	MIPLINEAR = GL_LINEAR_MIPMAP_LINEAR,
	CLAMP = GL_CLAMP_TO_EDGE,
	REPEAT = GL_REPEAT,
	MREPEAT = GL_MIRRORED_REPEAT
};

class OGLTexture final
{
public:
	OGLTexture() = default;
	explicit OGLTexture(
		const std::string& path,
		TextureOptions min = MIPLINEAR,
		TextureOptions mag = LINEAR,
		TextureOptions wrapS = REPEAT,
		TextureOptions wrapT = MREPEAT);
	~OGLTexture();

	void commit(
		const std::string& path,
		TextureOptions min = MIPLINEAR,
		TextureOptions mag = LINEAR,
		TextureOptions wrapS = REPEAT,
		TextureOptions wrapT = MREPEAT);
	void useTexture();
	void renderAsBackground();

	uint32_t texture{};
	std::string name;
};

class OGLEmptyTexture final
{
public:
	OGLEmptyTexture() = default;
	OGLEmptyTexture(
		TexType type, uint32_t width, uint32_t height,
		TextureOptions min = LINEAR,
		TextureOptions mag = LINEAR,
		TextureOptions wrapS = REPEAT,
		TextureOptions wrapT = REPEAT);
	~OGLEmptyTexture();

	void commit(
		TexType type, uint32_t width, uint32_t height,
		TextureOptions min = LINEAR,
		TextureOptions mag = LINEAR,
		TextureOptions wrapS = REPEAT,
		TextureOptions wrapT = REPEAT);
	void resize(uint32_t width, uint32_t height, char* data);

	uint32_t texture{};
	TexType type{};
};
