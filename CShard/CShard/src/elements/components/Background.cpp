#include "Background.hpp"

#include "../../device/graphics/Texture.hpp"
#include "../../engine/ResourceManager.hpp"

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

void Background::render(PhysicalData& parent)
{
	if (ResourceManager::isValidTexture(texID)) ResourceManager::getTexture(texID)->renderAsBackground(parent);
}
