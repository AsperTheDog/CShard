#include "Background.hpp"

#include "../../Engine.hpp"
#include "../../device/graphics/GTexture.hpp"

void Background::setTexture()
{
	this->texID = tempTexID;
}

void Background::render()
{
	if (Engine::isValidTexture(texID)) Engine::getTexture(texID)->renderAsBackground();
}
