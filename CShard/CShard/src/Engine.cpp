#include "Engine.hpp"
#include <device/window/SDLFramework.hpp>

Engine::Engine()
{

}

void Engine::init(GLibraries lib)
{
	SDLFramework::create(lib);
	GFramework::create(lib);

	SDLFramework::getInstance()->init();
	GFramework::getInstance()->init();
}

void Engine::run()
{
	while (SDLFramework::getInstance()->shouldWindowClose())
	{

	}
}
