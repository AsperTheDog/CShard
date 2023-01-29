#include "GTexture.hpp"

GTexture::GTexture(std::string filepath) : name(std::move(filepath))
{
}

GEmptyTexture::GEmptyTexture(TexType type) : type(type)
{
}
