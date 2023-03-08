#include "engine/Engine.hpp"
int main(int argc, char* argv[])
{ 
	char* filename = nullptr;
	bool isIDE = true;
	if (argc >= 2) filename = argv[1];
	if (argc == 3) isIDE = strcmp(argv[2], "noIDE") != 0;
	Engine::init(isIDE, filename);
	Engine::run();
	Engine::shutDown();
	return 0;
}