#include "GFramework.hpp"

#include <fstream>

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

GFramework* GFramework::get()
{
	return GFramework::instance;
}

void GFramework::deleteInstance()
{
	delete instance;
}

std::string GFramework::loadShaderSrc(const std::string& file)
{
	std::string src;
    std::fstream inFile;

    inFile.open(file);
    if (inFile.is_open())
    {
        std::string temp;
        while (std::getline(inFile, temp))
            src += temp + "\n";
    }
    else
    {
        throw std::exception("Error loading shader");
    }
    inFile.close();
    return src;
}
