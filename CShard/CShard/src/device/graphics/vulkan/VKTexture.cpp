#include "VKTexture.hpp"

VKTexture::VKTexture(std::string path) : GTexture(path)
{
}

VKTexture::~VKTexture()
{
}

void VKTexture::useTexture()
{
}

void VKTexture::renderAsBackground()
{
}

FBVKTexture::FBVKTexture(TexType type, uint32_t width, uint32_t height) : FBTexture(type)
{
}

void FBVKTexture::resize(uint32_t width, uint32_t height, char* data)
{
}
