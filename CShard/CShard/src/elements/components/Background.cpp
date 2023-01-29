#include "Background.hpp"

#include "../../device/graphics/opengl/OGLTexture.hpp"
#include "../../ResourceManager.hpp"

void Background::setTexture()
{
	this->texID = tempTexID;
}

void Background::serialize(std::ofstream& wf)
{
	wf.write((char*) &texID, sizeof(texID));
}

void Background::deserialize(std::ifstream& wf)
{
	wf.read((char*) &texID, sizeof(texID));
}

void Background::render()
{
	if (ResourceManager::isValidTexture(texID)) ResourceManager::getTexture(texID)->renderAsBackground();
}
