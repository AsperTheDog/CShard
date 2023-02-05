#pragma once
#include <fstream>

enum ScriptType
{
	SCRIPT_INIT,
	SCRIPT_FRAME,
	SCRIPT_DIE,
	SCRIPT_COLLISION_ENTER,
	SCRIPT_COLLISION_IN,
	SCRIPT_COLLISION_EXIT,
	SCRIPT_COLLISION_OUT
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

