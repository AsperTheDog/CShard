#pragma once

#include "Mesh.hpp"
#include "Texture.hpp"
#include "GraphicsHeader.hpp"

#include "../window/SDLFramework.hpp"
#include "PostEffects.hpp"

#define GLMAYOR 4
#define GLMINOR 5


class Light;
class Model;
struct PhysicalData;
class Camera;

enum ShaderType
{
	SHADER_BASE,
	SHADER_BACK,
	SHADER_POST
};

class GFramework
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

	static void create();

	static void initRender();
	static void endRender();
	static void resizeWindow();

	static void loadImGuiBackends();
	static void loadImGuiFrame();
	static void destroyImGui();
	static void renderImgui();

	static Shader* getBaseShader();
	static Shader* getBackShader();

	static void setDefaultTexture();
	static void loadCamUniforms(Camera& camera);
	static void loadModelUniforms(Model& mod, PhysicalData& pData, bool material);
	static void setPostUniforms();
	static uint32_t getImGuiTexture();
	static void resizeImGuiTextures();
	static void loadLightUniforms(Light& light, PhysicalData& parent);
	static void prepareShader(ShaderType type);

	inline static Shader* baseShader = nullptr;
	inline static Shader* backgroundShader = nullptr;
	inline static Shader* postShader = nullptr;

	inline static Texture defaultTex{};
	inline static PostQuad fullQuadMesh{};

	inline static glm::uvec2 imGuiSize{1920, 1080};
	inline static uint8_t lightSourceCount = 0;
	inline static Light* lights[10];

	inline static float postMult = 1.f;
	inline static FilmGrain filmGrain{};
	inline static bool postEffectsActive = false;
	inline static uint32_t activeShader = 0;

private:
	static std::string loadShaderSrc(const std::string& file);

	inline static glm::uvec2 viewPortSize{1920, 1080};
	inline static uint16_t lightCounter = 0;
	inline static SDL_GLContext gl_context = nullptr;
	inline static EmptyTexture baseTexture{};
	inline static EmptyTexture baseDepth{};
	inline static EmptyTexture postTexture{};
	inline static EmptyTexture postDepth{};
	inline static uint32_t baseFBO = 0;
	inline static uint32_t postFBO = 0;
};
