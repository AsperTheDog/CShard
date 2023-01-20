#pragma once

#include "OGLTexture.hpp"
#include "../GFramework.hpp"
#include "../../window/SDLFramework.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLMAYOR 4
#define GLMINOR 5

class OGLFramework final : public GFramework
{
public:
	class Shader
	{
	public:
		// Contrary to what the name of the variable may make you think,
		// this attribute holds the shader's ID
		GLuint id{};

		Shader(const std::string& vertex, const std::string& fragment);

		// Compiles a shader from a file to an OpenGL object
		static uint32_t loadShader(GLenum type, const std::string& file);

		// Simply picks up all shaders together and links them into an OpenGL pipeline
		void linkProgram(uint32_t vs, uint32_t fs);
	};

	void init() override;
	void initRender() override;
	void endRender() override;
	void resizeWindow() override;

	void loadImGuiBackends() override;
	void loadImGuiFrame() override;
	void destroyImGui() override;
	void renderImgui() override;

	Shader* getBaseShader();
	Shader* getBackShader();

	GMesh* createMesh(std::string filepath) override;
	GTexture* createTexture(std::string filepath) override;

	void setDefaultTexture() override;

private:

	SDL_GLContext gl_context = nullptr;
	OGLTexture defaultTex{DEFAULT_TEX_LOCATION};
	Shader* baseShader = nullptr;
	Shader* backgroundShader = nullptr;
};