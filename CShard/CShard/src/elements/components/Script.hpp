#pragma once
#include <fstream>

enum ScriptType
{
	SCRIPT_FRAME,
	SCRIPT_INIT,
	SCRIPT_DIE
};

class Script
{
public:
	Script();

	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	ScriptType type;
	uint32_t scriptID;
};

