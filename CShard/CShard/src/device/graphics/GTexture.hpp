#pragma once
#include <string>

class GTexture
{
public:
	explicit GTexture(std::string filepath);
	virtual ~GTexture() = default;
	
	virtual void renderAsBackground() = 0;
	virtual void useTexture() = 0;
	virtual void resize(uint32_t width, uint32_t height, char* data) = 0;

	std::string name;
};

