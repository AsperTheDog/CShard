#pragma once
#include <string>

#include "../GTexture.hpp"

class VKTexture : public GTexture
{
public:
	explicit VKTexture(std::string path);
	~VKTexture() override;

	void useTexture() override;
	void renderAsBackground() override;
};

