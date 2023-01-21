#pragma once
#include <vec3.hpp>

struct PhysicalData
{
	glm::vec3 pos{};
	glm::vec3 rot{};
	glm::vec3 scale{};
	bool changed = false;
};
