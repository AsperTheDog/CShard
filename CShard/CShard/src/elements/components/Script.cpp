#include "Script.hpp"
#include "../../engine/Config.hpp"

Script::Script()
{
}

void Script::serialize(std::ofstream& wf)
{
	wf.write((char*) &type, sizeof(type));
	wf.write((char*) &scriptID, sizeof(scriptID));
}

void Script::deserialize(std::ifstream& wf)
{
	wf.read((char*) &type, sizeof(type));
	wf.read((char*) &scriptID, sizeof(scriptID));
}
