#pragma once
#include <vec3.hpp>

#include "../PhysicalData.hpp"

class Light
{
public:
	Light();

	glm::vec3 getLightpos(PhysicalData& parent);

	float constant;
    float linear;
    float quadratic;

	glm::vec3 position;
	glm::vec3 color;
};

