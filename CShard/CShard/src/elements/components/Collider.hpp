#pragma once
#include <fstream>

enum ColliderType
{
	COLLIDER_SPHERE,
	COLLIDER_AABB,
	COLLIDER_OBB,
	COLLIDER_CYLINDER
};

class Collider
{
public:
	Collider();

	void testCollisions();
	
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	uint32_t enterScript;
	uint32_t insideScript;
	uint32_t exitScript;
	uint32_t outsideScript;
};

