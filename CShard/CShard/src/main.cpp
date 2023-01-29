#include "Engine.hpp"

int main(int argc, char *argv[])
{
	char* filename = nullptr;
	if (argc == 2) filename = argv[1];
	Engine::init(true, filename);
	Engine::run();
	Engine::shutDown();
}