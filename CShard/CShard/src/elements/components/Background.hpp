#pragma once
#include <string>

class GTexture;

class Background
{
public:
	void render();
	void setTexture();
	
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	int tempTexID{};
private:
	uint32_t texID = 0;
};

