#pragma once
#include <string>

#include "../GTexture.hpp"

enum TexType
{
	COLOR,
	DEPTH
};

class OGLTexture final : public GTexture
{
public:
	explicit OGLTexture(TexType type);
	explicit OGLTexture(const std::string& path);
	~OGLTexture() override;
	
	void useTexture() override;
	void renderAsBackground() override;
	void resize(uint32_t width, uint32_t height, char* data) override;

	uint32_t texture{};
private:
	TexType type;
};

