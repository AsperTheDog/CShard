#include "GFramework.hpp"
#include "opengl/OGLFramework.hpp"
#include "vulkan/VKFramework.hpp"

void GFramework::create(GLibraries lib)
{
	switch(lib)
	{
	case OPENGL:
		GFramework::instance = new OGLFramework();
		break;
	case VULKAN:
		GFramework::instance = new VKFramework();
		break;
	}
}

GFramework* GFramework::getInstance()
{
	return GFramework::instance;
}

void GFramework::deleteInstance()
{
	delete instance;
}
