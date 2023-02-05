#include "Script.hpp"

Script::Script() : type(SCRIPT_INIT), scriptID(0)
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
	tempScrID = scriptID;
}

void Script::setScript()
{
	scriptID = tempScrID;
}
