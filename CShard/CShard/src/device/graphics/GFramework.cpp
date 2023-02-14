#include "GFramework.hpp"

#include <SDL_syswm.h>
#include <sstream>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>

#include "Mesh.hpp"
#include "PostEffects.hpp"
#include "Texture.hpp"
#include "../../ide/ImGuiManager.hpp"
#include "../../engine/Engine.hpp"
#include "../../elements/components/Light.hpp"

/*
 * This debug callback function was made using the labhelper.cpp file as template
 * The file's origin is the following course:
 *  - Name: TDA362 / DIT224 Computer graphics 2022
 *  - Lecturer: Ulf Assarsson
 */
namespace
{
	GLvoid CALLBACK openGLDebugCallback(GLenum aSource, GLenum aType, GLuint aId, GLenum aSeverity,
		GLsizei, GLchar const* aMessage, GLvoid*)
	{
		const char* srcStr = nullptr;
		switch(aSource)
		{
		case GL_DEBUG_SOURCE_API:
			srcStr = "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			srcStr = "WINDOW_SYSTEM";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			srcStr = "SHADER_COMPILER";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			srcStr = "THIRD_PARTY";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			srcStr = "APPLICATION";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			srcStr = "OTHER";
			break;
		default:
			srcStr = "UNKNOWN";
			break;
		}
		
		const char* typeStr = nullptr;
		switch(aType)
		{
		case GL_DEBUG_TYPE_ERROR:
			typeStr = "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			typeStr = "DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			typeStr = "UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			typeStr = "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			typeStr = "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_OTHER:
			typeStr = "OTHER";
			break;
		default:
			typeStr = "UNKNOWN";
			break;
		}
		
		const char* sevStr = nullptr;
		switch(aSeverity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			sevStr = "HIGH";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			sevStr = "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			sevStr = "LOW";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			sevStr = "NOTIFICATION";
			break;
		default:
			sevStr = "UNKNOWN";
		}
		
		if(aSeverity != GL_DEBUG_SEVERITY_NOTIFICATION)
		{
			std::stringstream szs;
			szs << "\n"
			    << "--\n"
			    << "-- GL DEBUG MESSAGE:\n"
			    << "--   severity = '" << sevStr << "'\n"
			    << "--   type     = '" << typeStr << "'\n"
			    << "--   source   = '" << srcStr << "'\n"
			    << "--   id       = " << std::hex << aId << "\n"
			    << "-- message:\n"
			    << aMessage << "\n"
			    << "--\n"
			    << "\n";

			fprintf(stderr, "%s", szs.str().c_str());
			fflush(stderr);
#if defined(_WIN32)
			OutputDebugStringA(szs.str().c_str());
#endif
		}
		
		if(aSeverity == GL_DEBUG_SEVERITY_HIGH)
		{
#if defined(_WIN32)
			if(IsDebuggerPresent())
				__debugbreak();
#else
			raise(SIGTRAP);
#endif
		}
	}
#undef CALLBACK_
}

void GFramework::create()
{
    gl_context = SDL_GL_CreateContext(SDLFramework::getWindow());
	SDL_GL_MakeCurrent(SDLFramework::getWindow(), gl_context);
	
    glewExperimental = GL_TRUE; 
    if( glewInit() != GLEW_OK )
    {
        throw std::runtime_error("Error initializing GLEW!");
    }

	{
		windowSize = SDLFramework::getSize();
		GFramework::resizeWindow();
	}

	glDebugMessageCallback((GLDEBUGPROC)openGLDebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, false);

#ifndef NDEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	baseShader.commit(BASE_VERTEX_LOCATION, BASE_FRAGMENT_LOCATION);
	backgroundShader.commit(BACK_VERTEX_LOCATION, BACK_FRAGMENT_LOCATION);
	wireframeShader.commit(BASE_VERTEX_LOCATION, FRAME_FRAGMENT_LOCATION);
	
	FilmGrain::commitShader();
	Atmospheric::commitShader();
	BlackFade::commitShader();
	DepthEffect::commitShader();
	Pixelate::commitShader();

	defaultTex.commit(DEFAULT_TEX_LOCATION);
	PostQuad::init();

	baseFB.commit();
	postFB1.commit();

	screen.id = 0;
	screen.bind();
}

void GFramework::initRender()
{
	{
		baseFB.bind();
		if (windowSize != viewPortSize)
		{
			viewPortSize = windowSize;
			resizeFBs();
			GFramework::resizeWindow();
			Engine::activeCam->updateAspectRatio((float)viewPortSize.x / (float)viewPortSize.y);
		}
	}

	postPingPong.first = &baseFB;
	postPingPong.second = &postFB1;

	glUniform1ui(glGetUniformLocation(baseShader.id, "lightNum"), lightSourceCount);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GFramework::endRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	lightCounter = 0;
}

void GFramework::loadImGuiBackends()
{
	ImGui_ImplSDL2_InitForOpenGL(SDLFramework::getWindow(), gl_context);
	std::string glsl_version = "#version " + std::to_string(GLMAYOR) + std::to_string(GLMINOR) + "0";
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void GFramework::loadImGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void GFramework::destroyImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GFramework::renderImgui()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGuiManager::getIO()->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

Shader* GFramework::getBaseShader()
{
	return &baseShader;
}

Shader* GFramework::getBackShader()
{
	return &backgroundShader;
}

void GFramework::setDefaultTexture()
{
	defaultTex.useTexture();
}

void GFramework::loadCamUniforms(Camera& camera)
{
	glUniform3fv(glGetUniformLocation(activeShader, "cam.pos"), 1, &camera.pos.x);
	glUniform3fv(glGetUniformLocation(activeShader, "cam.dir"), 1, &camera.dir.x);
}

void GFramework::loadModelUniforms(Camera& camera, Model& mod, PhysicalData& pData, bool material)
{
	glUniformMatrix4fv(glGetUniformLocation(activeShader, "model.mat"), 1, false, &mod.getModelMatrix(pData)[0].x);
	glUniformMatrix4fv(glGetUniformLocation(activeShader, "model.invMat"), 1, false, &mod.getInvModelMatrix(pData)[0].x);
	glm::mat4 mvpMat = camera.getProjMatrix() * camera.getViewMatrix(pData) * mod.getModelMatrix(pData);
	glUniformMatrix4fv(glGetUniformLocation(activeShader, "MVPmat"), 1, false, &mvpMat[0].x);
	if (material)
	{
		glUniform1f(glGetUniformLocation(activeShader, "mat.shininess"), mod.mat.shininess);
		glUniform1f(glGetUniformLocation(activeShader, "mat.emission"), mod.mat.emission);
	}
}

uint32_t GFramework::getImGuiTexture()
{
	return postPingPong.first->color.texture;
}

void GFramework::resizeFBs()
{
	baseFB.resize(viewPortSize.x, viewPortSize.y);
	postFB1.resize(viewPortSize.x, viewPortSize.y);
}

void GFramework::loadLightUniforms(Light& light, PhysicalData& parent)
{
	glm::vec3 pos = light.getLightpos(parent);
	glUniform1f(glGetUniformLocation(activeShader, ("pLights[" + std::to_string(lightCounter) + "].constant").c_str()), light.constant);
	glUniform1f(glGetUniformLocation(activeShader, ("pLights[" + std::to_string(lightCounter) + "].linear").c_str()), light.linear);
	glUniform1f(glGetUniformLocation(activeShader, ("pLights[" + std::to_string(lightCounter) + "].quadratic").c_str()), light.quadratic);
	glUniform3fv(glGetUniformLocation(activeShader, ("pLights[" + std::to_string(lightCounter) + "].position").c_str()), 1, &pos.x);
	glUniform3fv(glGetUniformLocation(activeShader, ("pLights[" + std::to_string(lightCounter) + "].color").c_str()), 1, &light.color.x);
	lightCounter++;
}

void GFramework::prepareShader(ShaderType type)
{
	switch (type)
	{
	case SHADER_BACK:
		glUseProgram(backgroundShader.id);
		activeShader = backgroundShader.id;
		baseFB.bind();
		break;
	case SHADER_BASE:
		glUseProgram(baseShader.id);
		activeShader = baseShader.id;
		baseFB.bind();
		break;
	case SHADER_WIRE:
		glUseProgram(wireframeShader.id);
		activeShader = wireframeShader.id;
		postPingPong.first->bind();
	}
}

void GFramework::resizeWindow()
{
	glViewport(0, 0, viewPortSize.x, viewPortSize.y);
}
