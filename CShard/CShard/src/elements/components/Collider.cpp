#include "Collider.hpp"
#include "../../engine/Config.hpp"

Collider::Collider()
{
	
}

void Collider::testCollisions()
{
	
}

void Collider::serialize(std::ofstream& wf)
{
	
	wf.write((char*) &enterScript, sizeof(enterScript));
	wf.write((char*) &insideScript, sizeof(insideScript));
	wf.write((char*) &exitScript, sizeof(exitScript));
	wf.write((char*) &outsideScript, sizeof(outsideScript));	
}

void Collider::deserialize(std::ifstream& wf)
{
	wf.read((char*) &enterScript, sizeof(enterScript));
	wf.read((char*) &insideScript, sizeof(insideScript));
	wf.read((char*) &exitScript, sizeof(exitScript));
	wf.read((char*) &outsideScript, sizeof(outsideScript));	
}
