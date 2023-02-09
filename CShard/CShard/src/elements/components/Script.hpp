#pragma once
#include <fstream>

enum ScriptType
{
	SCRIPT_INIT = -3,
	SCRIPT_FRAME = -2,
	SCRIPT_DIE = -1,
	SCRIPT_COLLISION_ENTER = 0,
	SCRIPT_COLLISION_IN = 1,
	SCRIPT_COLLISION_EXIT = 2,
	SCRIPT_COLLISION_OUT = 3
};

class Script
{
public:
	Script();

	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);
	void setScript();

	ScriptType type;
	int tempScrType = 0;
	uint32_t scriptID;
	int tempScrID = 0;
};

