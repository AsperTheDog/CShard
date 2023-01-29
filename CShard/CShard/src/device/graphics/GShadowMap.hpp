#pragma once
#include <vec3.hpp>

class GShadowMap
{
public:
	virtual ~GShadowMap() = default;

	virtual void render(unsigned index, glm::vec3 lightPos) = 0;
};

