#include "Texture.hpp"

#include <stdexcept>

#include "GFramework.hpp"
#include "../../engine/Engine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../../elements/components/Camera.hpp"
#include "Mesh.hpp"

Texture::Texture(
	const std::string& path, 
	TextureOptions min,
	TextureOptions mag, 
	TextureOptions wrapS, 
	TextureOptions wrapT)
	: name(path)
{
	commit(path, min, mag, wrapS, wrapT);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

void Texture::commit(
	const std::string& path, 
	TextureOptions min, 
	TextureOptions mag, 
	TextureOptions wrapS,
	TextureOptions wrapT)
{
	this->name = path;
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
	if (!data)
	{
		throw std::runtime_error("Could not load texture");
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	stbi_image_free(data);
}

void Texture::useTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::renderAsBackground(PhysicalData& parent)
{
	this->useTexture();

	GLuint program = GFramework::getBackShader()->id;
	glUseProgram(program);
	glm::mat4 invPV = glm::inverse(Engine::activeCam->getProjMatrix() * Engine::activeCam->getViewMatrix(parent));
	glUniformMatrix4fv(glGetUniformLocation(program, "invPV"), 1, false, &invPV[0].x);
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, &Engine::activeCam->pos.x);

	PostQuad::render();
}

void EmptyTexture::commit(
	TexType type, uint32_t width, uint32_t height, 
	TextureOptions min, 
	TextureOptions mag,
	TextureOptions wrapS, 
	TextureOptions wrapT)
{
	this->type = type;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	if (type == COLOR)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
}

EmptyTexture::EmptyTexture(
	TexType type, uint32_t width, uint32_t height,
	TextureOptions min,
	TextureOptions mag, 
	TextureOptions wrapS, 
	TextureOptions wrapT)
	: type(type)
{
	commit(type, width, height, min, mag, wrapS, wrapT);
}

EmptyTexture::~EmptyTexture()
{
	glDeleteTextures(1, &texture);
}

void EmptyTexture::resize(uint32_t width, uint32_t height, char* data)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	if (type == COLOR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
}
