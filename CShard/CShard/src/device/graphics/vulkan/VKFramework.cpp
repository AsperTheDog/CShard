#include "VKFramework.hpp"

#include "VKMesh.hpp"
#include "VKTexture.hpp"

void VKFramework::init()
{
}

void VKFramework::initRender()
{
}

void VKFramework::endRender()
{
}

void VKFramework::loadImGuiBackends()
{
}

void VKFramework::loadImGuiFrame()
{
}

void VKFramework::destroyImGui()
{
}

void VKFramework::renderImgui()
{
}

GMesh* VKFramework::createMesh(std::string filepath)
{
	return new VKMesh(filepath);
}

GTexture* VKFramework::createTexture(std::string filepath)
{
	return new VKTexture(filepath);
}

void VKFramework::setDefaultTexture()
{
}

void VKFramework::resizeWindow()
{
}
