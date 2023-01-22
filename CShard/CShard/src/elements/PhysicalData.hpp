#pragma once
#include <vec3.hpp>

struct PhysicalData
{
	glm::vec3 pos{0};
	glm::vec3 rot{0};
	glm::vec3 scale{1};
	bool changed = true;
};
