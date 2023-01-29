#pragma once
#include <fstream>

class Script
{
public:
	Script();

	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);
};

