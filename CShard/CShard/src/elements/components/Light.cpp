#include "Light.hpp"

Light::Light()
	: constant(1.f), linear(0.14f), quadratic(0.07f), position{0.f}, color{1.f}
{
}

glm::vec3 Light::getLightpos(PhysicalData& parent)
{
	return this->position + parent.pos;
}
