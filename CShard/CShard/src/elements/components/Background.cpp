#include "Background.hpp"

#include "../../device/graphics/GFramework.hpp"

#include <utility>


void Background::setTexture(std::string filepath)
{
	this->tex = GFramework::get()->createTexture(std::move(filepath));
}

void Background::render()
{
	if (tex) tex->renderAsBackground();
}
