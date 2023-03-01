#pragma once
#include <string>

#include "GraphicsHeader.hpp"
#include "../../engine/AssetPath.hpp"

struct PhysicalData;

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
		const AssetPath& path,
		TextureOptions min = MIPLINEAR,
		TextureOptions mag = LINEAR,
		TextureOptions wrapS = REPEAT,
		TextureOptions wrapT = MREPEAT);
	~Texture();

	void commit(
		const AssetPath& source,
		TextureOptions min = MIPLINEAR,
		TextureOptions mag = LINEAR,
		TextureOptions wrapS = REPEAT,
		TextureOptions wrapT = MREPEAT);
	void commit();
	void useTexture();
	void renderAsBackground(PhysicalData& parent);
	void rename(std::filesystem::path& newName);
	void reload();

	uint32_t texture{};

	std::string name;
	std::filesystem::path source;
	bool isTracked = false;

	TextureOptions min = MIPLINEAR;
	TextureOptions mag = LINEAR;
	TextureOptions wrapS = REPEAT;
	TextureOptions wrapT = MREPEAT;
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
