#include "VKFramework.hpp"

#include "VKMesh.hpp"
#include "VKTexture.hpp"

VKFramework::VKFramework()
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

void VKFramework::loadCamUniforms(Camera& camera)
{
}

void VKFramework::loadModelUniforms(Model& mod, PhysicalData& pData, bool material)
{
}

uint32_t VKFramework::getImGuiTexture()
{
	return 0;
}

GCubeTexture* VKFramework::createCubeTexture(uint32_t width, uint32_t height)
{
	return nullptr;
}

void VKFramework::loadLightUniforms(Light& light, PhysicalData&)
{
}

void VKFramework::setPostUniforms()
{
}

void VKFramework::prepareShader(ShaderType type)
{
}

void VKFramework::resizeWindow()
{
}
