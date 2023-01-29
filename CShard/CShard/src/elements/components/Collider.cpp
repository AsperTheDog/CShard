#include "Collider.hpp"

Collider::Collider()
{
	
}

void Collider::testCollisions()
{
	
}

void Collider::serialize(std::ofstream& wf)
{
	wf.write((char*) &hasCollided, sizeof(hasCollided));
}

void Collider::deserialize(std::ifstream& wf)
{
	wf.read((char*) &hasCollided, sizeof(hasCollided));
}
