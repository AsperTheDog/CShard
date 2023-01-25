#pragma once

#include "../GraphicsHeader.hpp"

#include "../GFramework.hpp"
#include "../../window/SDLFramework.hpp"

#define GLMAYOR 4
#define GLMINOR 5

struct PhysicalData;
class OGLEmptyTexture;
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
	void loadCamUniforms(Camera& camera) override;
	void loadModelUniforms(Model& mod) override;
	uint32_t getImGuiTexture() override;
	void resizeImGuiTextures();
	GCubeTexture* createCubeTexture(uint32_t width, uint32_t height) override;
	void loadLightUniforms(Light& light, PhysicalData& parent) override;

	Shader* baseShader = nullptr;
	Shader* backgroundShader = nullptr;
private:
	uint16_t lightCounter = 0;
	SDL_GLContext gl_context = nullptr;
	OGLEmptyTexture* imGuiTexture;
	OGLEmptyTexture* imGuiDepth;
	uint32_t imGuiFBO = 0;
};
