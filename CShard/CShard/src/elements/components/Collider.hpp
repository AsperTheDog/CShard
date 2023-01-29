#pragma once
#include <fstream>

class Collider
{
public:
	Collider();

	void testCollisions();
	
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	bool hasCollided;
};

