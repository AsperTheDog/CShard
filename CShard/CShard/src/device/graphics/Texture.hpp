#pragma once
#include <string>

#include "GraphicsHeader.hpp"

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

class Texture final
{
public:
	Texture() = default;
	explicit Texture(
		const std::string& path,
		TextureOptions min = MIPLINEAR,
		TextureOptions mag = LINEAR,
		TextureOptions wrapS = REPEAT,
		TextureOptions wrapT = MREPEAT);
	~Texture();

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

class EmptyTexture final
{
public:
	EmptyTexture() = default;
	EmptyTexture(
		TexType type, uint32_t width, uint32_t height,
		TextureOptions min = LINEAR,
		TextureOptions mag = LINEAR,
		TextureOptions wrapS = REPEAT,
		TextureOptions wrapT = REPEAT);
	~EmptyTexture();

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