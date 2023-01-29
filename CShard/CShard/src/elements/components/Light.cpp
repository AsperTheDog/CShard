#include "Light.hpp"
#include "../../device/graphics/GFramework.hpp"

Light::Light()
	: constant(1.f), linear(0.14f), quadratic(0.07f), position{0.f}, color{1.f}
{
	shadowMap = GFramework::get()->createShadowMap(1024);
}

glm::vec3 Light::getLightpos(PhysicalData& parent)
{
	return this->position + parent.pos;
}

void Light::reloadShadowMap(uint32_t index, PhysicalData& parent)
{
	shadowMap->render(index, this->position + parent.pos);
}
