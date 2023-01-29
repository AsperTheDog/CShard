#include "Script.hpp"

Script::Script()
{
}

void Script::serialize(std::ofstream& wf)
{
	uint8_t tmp = 0;
	wf.write((char*) &tmp, sizeof(uint8_t));
}

void Script::deserialize(std::ifstream& wf)
{
	uint8_t tmp;
	wf.read((char*) &tmp, sizeof(uint8_t));
}
