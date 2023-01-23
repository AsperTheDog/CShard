#pragma once
#include <string>

#include "../GTexture.hpp"

class VKTexture final : public GTexture
{
public:
	explicit VKTexture(std::string path);
	~VKTexture() override;

	void useTexture() override;
	void renderAsBackground() override;
};

class FBVKTexture final : public GEmptyTexture
{
public:
	explicit FBVKTexture(TexType type, uint32_t width, uint32_t height);
	void resize(uint32_t width, uint32_t height, char* data) override;
};
