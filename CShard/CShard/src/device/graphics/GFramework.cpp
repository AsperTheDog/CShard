#include "GFramework.hpp"

#include <fstream>

#include "opengl/OGLFramework.hpp"
#include "opengl/OGLMesh.hpp"
#include "opengl/OGLTexture.hpp"
#include "vulkan/VKFramework.hpp"
#include "vulkan/VKMesh.hpp"
#include "vulkan/VKTexture.hpp"

#include "../../Config.hpp"

GLibraries GFramework::type;
GTexture* GFramework::defaultTex;
BackGMesh* GFramework::backgroundMesh;

void GFramework::create(GLibraries lib)
{
    GFramework::type = lib;
	switch(lib)
	{
	case OPENGL:
		GFramework::instance = new OGLFramework();
        defaultTex = new OGLTexture(DEFAULT_TEX_LOCATION);
        backgroundMesh = new BackOGLMesh();
		break;
	case VULKAN:
		GFramework::instance = new VKFramework();
        defaultTex = new VKTexture(DEFAULT_TEX_LOCATION);
        backgroundMesh = new BackVKMesh();
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
        SDLFramework::showErrorMessage(
            "Could not load shader data", 
            "Make sure the pak folder is not missing in the program folder");
    	throw std::exception("Error loading shader");
    }
    inFile.close();
    return src;
}
