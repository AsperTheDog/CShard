#include "GTexture.hpp"

GTexture::GTexture(std::string filepath) : name(filepath)
{
}

GEmptyTexture::GEmptyTexture(TexType type) : type(type)
{
}
