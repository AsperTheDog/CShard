#include "OGLFramework.hpp"


#if defined(_WIN32)
	#include <Windows.h>
#endif

#include <stdexcept>
#include <string>

#include <sstream>

#include "OGLMesh.hpp"
#include "OGLTexture.hpp"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl.h"
#include "../../../ide/ImGuiManager.hpp"

/*
 * This debug callback function was made using the labhelper.cpp file as template
 * The file's origin is the following course:
 *  - Name: TDA362 / DIT224 Computer graphics 2022
 *  - Lecturer: Ulf Assarsson
 */
namespace
{
#if defined(_WIN32)
#define CALLBACK_ CALLBACK
#else
#define CALLBACK_
#endif
	
	GLvoid CALLBACK_ openGLDebugCallback(GLenum aSource, GLenum aType, GLuint aId, GLenum aSeverity,
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

void OGLFramework::init()
{
    gl_context = SDL_GL_CreateContext(SDLFramework::getWindow());
	SDL_GL_MakeCurrent(SDLFramework::getWindow(), gl_context);
	
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        throw std::runtime_error("Error initializing GLEW!");
    }

	OGLFramework::resizeWindow();

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

	this->baseShader = new Shader(BASE_VERTEX_LOCATION, BASE_FRAGMENT_LOCATION);
	this->backgroundShader = new Shader(BACK_VERTEX_LOCATION, BACK_FRAGMENT_LOCATION);
}

void OGLFramework::initRender()
{
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OGLFramework::endRender()
{
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

void OGLFramework::setDefaultTexture()
{
	this->defaultTex.useTexture();
}

void OGLFramework::resizeWindow()
{
	int width, height;
	SDL_GetWindowSize(SDLFramework::getWindow(), &width, &height);
	glViewport(0, 0, width, height);
}

OGLFramework::Shader::Shader(const std::string& vertex, const std::string& fragment)
{
	uint32_t gs = 0;

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