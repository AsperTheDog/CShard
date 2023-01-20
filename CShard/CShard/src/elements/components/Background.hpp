#pragma once
#include <string>

class GTexture;

class Background
{
public:
	void render();
	void setTexture(std::string filepath);
private:
	GTexture* tex = nullptr;
};

