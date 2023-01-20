#include "Engine.hpp"

int main()
{
	Engine::init(OPENGL, true);
	Engine::run();
	Engine::shutDown();
}