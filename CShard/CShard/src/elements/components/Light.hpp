#pragma once
#include <fstream>
#include <vec3.hpp>

#include "../PhysicalData.hpp"

class GShadowMap;

class Light
{
public:
	Light();

	glm::vec3 getLightpos(PhysicalData& parent);
	
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	float constant;
    float linear;
    float quadratic;

	glm::vec3 position;
	glm::vec3 color;
};

