#include "Engine.hpp"

int main()
{
	Engine::init(OPENGL);
	Engine::run();
	Engine::shutDown();
	return 0;
}