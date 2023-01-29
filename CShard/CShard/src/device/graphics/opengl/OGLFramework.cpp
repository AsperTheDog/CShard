#include "OGLFramework.hpp"

#include <iostream>
#include <SDL_syswm.h>

#include <sstream>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>

#include "OGLMesh.hpp"
#include "OGLShadowMap.hpp"
#include "OGLTexture.hpp"
#include "../../../ide/ImGuiManager.hpp"
#include "../../../Engine.hpp"
#include "../../../elements/components/Light.hpp"

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

OGLFramework::OGLFramework()
{
    gl_context = SDL_GL_CreateContext(SDLFramework::getWindow());
	SDL_GL_MakeCurrent(SDLFramework::getWindow(), gl_context);
	
    glewExperimental = GL_TRUE; 
    if( glewInit() != GLEW_OK )
    {
        throw std::runtime_error("Error initializing GLEW!");
    }

	{
		glm::ivec2 size = SDLFramework::getSize();
		OGLFramework::resizeWindow();
	}

	glDebugMessageCallback((GLDEBUGPROC)openGLDebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, false);
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

	this->baseShader = new Shader(BASE_VERTEX_LOCATION, BASE_FRAGMENT_LOCATION);
	this->backgroundShader = new Shader(BACK_VERTEX_LOCATION, BACK_FRAGMENT_LOCATION);
	this->postShader = new Shader(POST_VERTEX_LOCATION, POST_FRAGMENT_LOCATION);
	this->shadowShader = new Shader(SHADOW_VERTEX_LOCATION, SHADOW_FRAGMENT_LOCATION);

	this->baseTexture = new OGLEmptyTexture(COLOR, 1920, 1080);
	this->baseDepth = new OGLEmptyTexture(DEPTH, 1920, 1080);
	glGenFramebuffers(1, &baseFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, baseFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->baseTexture->texture, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->baseDepth->texture, 0);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error(R"(BaseFramebuffer could not be completed)");
	}

	this->postTexture = new OGLEmptyTexture(COLOR, 1920, 1080);
	this->postDepth = new OGLEmptyTexture(DEPTH, 1920, 1080);
	glGenFramebuffers(1, &postFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->postTexture->texture, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->postDepth->texture, 0);
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error(R"(PostFramebuffer could not be completed)");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OGLFramework::initRender()
{
	if (Engine::isIDE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, baseFBO);
		if (imGuiSize != viewPortSize)
		{
			viewPortSize = imGuiSize;
			this->resizeImGuiTextures();
			OGLFramework::resizeWindow();
			Engine::activeCam->updateAspectRatio((float)viewPortSize.x / (float)viewPortSize.y);
		}
	}
	glUniform1ui(glGetUniformLocation(this->baseShader->id, "lightNum"), lightSourceCount);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLFramework::endRender()
{
	prepareShader(SHADER_POST);
	this->setPostUniforms();
	glBindFramebuffer(GL_FRAMEBUFFER, postFBO);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->baseTexture->texture);
	GFramework::fullQuadMesh->render();

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	lightCounter = 0;
}

void OGLFramework::loadImGuiBackends()
{
	ImGui_ImplSDL2_InitForOpenGL(SDLFramework::getWindow(), gl_context);
	std::string glsl_version = "#version " + std::to_string(GLMAYOR) + std::to_string(GLMINOR) + "0";
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void OGLFramework::loadImGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void OGLFramework::destroyImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void OGLFramework::renderImgui()
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

OGLFramework::Shader* OGLFramework::getBaseShader()
{
	return this->baseShader;
}

OGLFramework::Shader* OGLFramework::getBackShader()
{
	return this->backgroundShader;
}

GMesh* OGLFramework::createMesh(std::string filepath)
{
	return new OGLMesh(filepath);
}

GTexture* OGLFramework::createTexture(std::string filepath)
{
	return new OGLTexture(filepath);
}

GShadowMap* OGLFramework::createShadowMap(uint32_t size)
{
	return new OGLShadowMap(size);
}

void OGLFramework::setDefaultTexture()
{
	GFramework::defaultTex->useTexture();
}

void OGLFramework::loadCamUniforms(Camera& camera)
{
	glUniformMatrix4fv(glGetUniformLocation(GFramework::activeShader, "camMats.vMatrix"), 1, false, &camera.getViewMatrix()[0].x);
	glUniformMatrix4fv(glGetUniformLocation(GFramework::activeShader, "camMats.pMatrix"), 1, false, &camera.getProjMatrix()[0].x);
	glUniform3fv(glGetUniformLocation(GFramework::activeShader, "cam.pos"), 1, &camera.pos.x);
	glUniform3fv(glGetUniformLocation(GFramework::activeShader, "cam.dir"), 1, &camera.dir.x);
}

void OGLFramework::loadModelUniforms(Model& mod, bool material)
{
	glUniformMatrix4fv(glGetUniformLocation(GFramework::activeShader, "model.mat"), 1, false, &mod.modelMatrix[0].x);
	glUniformMatrix4fv(glGetUniformLocation(GFramework::activeShader, "model.invMat"), 1, false, &mod.invModelMatrix[0].x);
	if (material)
	{
		glUniform1f(glGetUniformLocation(GFramework::activeShader, "mat.shininess"), mod.mat.shininess);
		glUniform1f(glGetUniformLocation(GFramework::activeShader, "mat.emission"), mod.mat.emission);
	}
}

void OGLFramework::setPostUniforms()
{
	glUniform1ui(glGetUniformLocation(GFramework::activeShader, "current"), postEffectsActive ? filmGrain.id : 0);
	glUniform1f(glGetUniformLocation(GFramework::activeShader, "randomSeed"), filmGrain.nextNum);
	glUniform1f(glGetUniformLocation(GFramework::activeShader, "grainIntensity"), filmGrain.intensity);
	filmGrain.next();
	glUniform1f(glGetUniformLocation(GFramework::activeShader, "mult"), postMult);
}

uint32_t OGLFramework::getImGuiTexture()
{
	return this->postTexture->texture;
}

void OGLFramework::resizeImGuiTextures()
{
	this->baseTexture->resize(viewPortSize.x, viewPortSize.y, nullptr);
	this->baseDepth->resize(viewPortSize.x, viewPortSize.y, nullptr);
	this->postTexture->resize(viewPortSize.x, viewPortSize.y, nullptr);
	this->postDepth->resize(viewPortSize.x, viewPortSize.y, nullptr);
}

GCubeTexture* OGLFramework::createCubeTexture(uint32_t width, uint32_t height)
{
	return new OGLCubeTexture(width, height);
}

void OGLFramework::loadLightUniforms(Light& light, PhysicalData& parent)
{
	glm::vec3 pos = light.getLightpos(parent);
	glUniform1f(glGetUniformLocation(GFramework::activeShader, ("pLights[" + std::to_string(lightCounter) + "].constant").c_str()), light.constant);
	glUniform1f(glGetUniformLocation(GFramework::activeShader, ("pLights[" + std::to_string(lightCounter) + "].linear").c_str()), light.linear);
	glUniform1f(glGetUniformLocation(GFramework::activeShader, ("pLights[" + std::to_string(lightCounter) + "].quadratic").c_str()), light.quadratic);
	glUniform3fv(glGetUniformLocation(GFramework::activeShader, ("pLights[" + std::to_string(lightCounter) + "].position").c_str()), 1, &pos.x);
	glUniform3fv(glGetUniformLocation(GFramework::activeShader, ("pLights[" + std::to_string(lightCounter) + "].color").c_str()), 1, &light.color.x);
	lightCounter++;
}

void OGLFramework::prepareShader(ShaderType type)
{
	switch (type)
	{
	case SHADER_BACKGROUND:
		glUseProgram(backgroundShader->id);
		GFramework::activeShader = backgroundShader->id;
		glBindFramebuffer(GL_FRAMEBUFFER, baseFBO);
		break;
	case SHADER_BASE:
		glUseProgram(baseShader->id);
		GFramework::activeShader = baseShader->id;
		glBindFramebuffer(GL_FRAMEBUFFER, baseFBO);
		break;
	case SHADER_POST:
		glUseProgram(postShader->id);
		GFramework::activeShader = postShader->id;
		glBindFramebuffer(GL_FRAMEBUFFER, postFBO);
		break;
	case SHADER_SHADOW:
		glUseProgram(shadowShader->id);
		GFramework::activeShader = shadowShader->id;
		break;
	default: ;
	}
}

void OGLFramework::resizeWindow()
{
	glViewport(0, 0, viewPortSize.x, viewPortSize.y);
}

OGLFramework::Shader::Shader(const std::string& vertex, const std::string& fragment)
{
	// create shaders then link them into a pipeline
	uint32_t vs = loadShader(GL_VERTEX_SHADER, vertex);
	uint32_t fs = loadShader(GL_FRAGMENT_SHADER, fragment);

	OGLFramework::Shader::linkProgram(vs, fs);

	// once the pipeline is done we don't need the shader objects anymore
	glDeleteShader(vs);
	glDeleteShader(fs);
}

uint32_t OGLFramework::Shader::loadShader(GLenum type, const std::string& file)
{
	GLint success;

	uint32_t shaderID = glCreateShader(type);
	std::string src = GFramework::loadShaderSrc(file);
	const GLchar* shSrc = src.c_str();
	glShaderSource(shaderID, 1, &shSrc, nullptr);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
		throw std::runtime_error("Error while compiling shader");
	}

	return shaderID;
}

void OGLFramework::Shader::linkProgram(uint32_t vs, uint32_t fs)
{
	GLint success;

	this->id = glCreateProgram();
	glAttachShader(this->id, vs);
	glAttachShader(this->id, fs);

	glLinkProgram(this->id);

	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(this->id, 512, nullptr, infoLog);
		glDeleteShader(vs);
		glDeleteShader(fs);
		throw std::runtime_error("Error linking program");
	}
}
