#pragma once
#include <string>

class GTexture;

class Background
{
public:
	void render();
	void setTexture();

	int tempTexID{};
private:
	uint32_t texID = 0;
};

