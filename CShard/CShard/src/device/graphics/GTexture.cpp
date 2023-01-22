#include "GTexture.hpp"

GTexture::GTexture(std::string filepath) : name(filepath)
{
}

FBTexture::FBTexture(TexType type) : type(type)
{
}
