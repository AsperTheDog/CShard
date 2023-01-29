#include "Light.hpp"
#include "../../device/graphics/GFramework.hpp"

Light::Light()
	: constant(1.f), linear(0.14f), quadratic(0.07f), position{0.f}, color{1.f}
{

}

glm::vec3 Light::getLightpos(PhysicalData& parent)
{
	return this->position + parent.pos;
}

void Light::serialize(std::ofstream& wf)
{
	wf.write((char*) &constant, sizeof(constant));
	wf.write((char*) &linear, sizeof(linear));
	wf.write((char*) &quadratic, sizeof(quadratic));
	wf.write((char*) &position, sizeof(position));
	wf.write((char*) &color, sizeof(color));
}

void Light::deserialize(std::ifstream& wf)
{
	wf.read((char*) &constant, sizeof(constant));
	wf.read((char*) &linear, sizeof(linear));
	wf.read((char*) &quadratic, sizeof(quadratic));
	wf.read((char*) &position, sizeof(position));
	wf.read((char*) &color, sizeof(color));
}
