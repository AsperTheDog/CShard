#pragma once

#include "FrameBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

#define GLMAYOR 4
#define GLMINOR 5


class Shader;
class Light;
class Model;
struct PhysicalData;
class Camera;

enum ShaderType
{
	SHADER_BASE,
	SHADER_BACK
};

class GFramework
{
public:

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
	static void loadModelUniforms(Camera& camera, Model& mod, PhysicalData& pData, bool material);
	static uint32_t getImGuiTexture();
	static void resizeFBs();
	static void loadLightUniforms(Light& light, PhysicalData& parent);
	static void prepareShader(ShaderType type);

	inline static Shader baseShader{};
	inline static Shader backgroundShader{};

	inline static Texture defaultTex{};

	inline static glm::uvec2 windowSize{1920, 1080};
	inline static uint8_t lightSourceCount = 0;
	inline static Light* lights[10];
	
	inline static uint32_t activeShader = 0;
	
	inline static glm::uvec2 viewPortSize{1920, 1080};
	inline static std::pair<FrameBuffer*, FrameBuffer*> postPingPong;
	
	inline static FrameBuffer screen;
	inline static FrameBuffer baseFB;
	inline static FrameBuffer postFB1;
private:
	inline static uint16_t lightCounter = 0;
	inline static SDL_GLContext gl_context = nullptr;
};
