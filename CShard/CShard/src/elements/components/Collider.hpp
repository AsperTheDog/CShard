#pragma once
#include <cstdint>
#include <glm.hpp>

enum ColliderType
{
	COLLIDE_SPHERE,
	COLLIDE_AABB
};

struct ColliderData
{
	ColliderType type;
	union Types
	{
		float radius;
		glm::vec3 size;
	};
	Types val;
};

class Collider
{
public:
	explicit Collider(ColliderData data);

	void testCollisions();

	bool hasCollided;
};

