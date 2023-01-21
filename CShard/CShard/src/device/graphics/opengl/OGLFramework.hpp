#pragma once

#include "../GraphicsHeader.hpp"

#include "../GFramework.hpp"
#include "../../window/SDLFramework.hpp"

#define GLMAYOR 4
#define GLMINOR 5

class OGLTexture;
class Model;

class OGLFramework final : public GFramework
{
public:
	class Shader
	{
	public:
		GLuint id{};

		Shader(const std::string& vertex, const std::string& fragment);
		static uint32_t loadShader(GLenum type, const std::string& file);
		void linkProgram(uint32_t vs, uint32_t fs);
	};

	OGLFramework();

	void initRender() override;
	void endRender() override;
	void resizeWindow(int width, int height) override;

	void loadImGuiBackends() override;
	void loadImGuiFrame() override;
	void destroyImGui() override;
	void renderImgui() override;

	Shader* getBaseShader();
	Shader* getBackShader();

	GMesh* createMesh(std::string filepath) override;
	GTexture* createTexture(std::string filepath) override;

	void setDefaultTexture() override;
	void loadCamUniforms(Camera* camera) override;
	void loadModelUniforms(Model* mod, glm::mat4& parent) override;
	uint32_t getImGuiTexture() override;

	Shader* baseShader = nullptr;
	Shader* backgroundShader = nullptr;
private:

	SDL_GLContext gl_context = nullptr;
	OGLTexture* imGuiTexture;
	OGLTexture* imGuiDepth;
	uint32_t imGuiFBO = 0;
};
