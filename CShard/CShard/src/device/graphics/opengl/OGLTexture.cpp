#include "OGLTexture.hpp"

#include <stdexcept>

#include "OGLFramework.hpp"
#include "../GraphicsHeader.hpp"
#include "../../../Engine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../GMesh.hpp"
#include "../../../elements/components/Camera.hpp"

OGLTexture::OGLTexture(const std::string& path) : GTexture(path)
{
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
	if (!data)
	{
		throw std::runtime_error("Could not load texture");
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

OGLTexture::~OGLTexture()
{
	glDeleteTextures(1, &texture);
}

void OGLTexture::useTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void OGLTexture::renderAsBackground()
{
	this->useTexture();

	GLuint program = ((OGLFramework*)GFramework::get())->backgroundShader->id;
	glUseProgram(program);
	glm::mat4 invPV = glm::inverse(Engine::activeCam->getProjMatrix() * Engine::activeCam->getViewMatrix());
	glUniformMatrix4fv(glGetUniformLocation(program, "invPV"), 1, false, &invPV[0].x);
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, &Engine::activeCam->pos.x);

	GFramework::backgroundMesh->render();
}

FBOGLTexture::FBOGLTexture(TexType type, uint32_t width, uint32_t height) : FBTexture(type)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (type == COLOR)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
}

FBOGLTexture::~FBOGLTexture()
{

}

void FBOGLTexture::resize(uint32_t width, uint32_t height, char* data)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	if (type == COLOR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
}
