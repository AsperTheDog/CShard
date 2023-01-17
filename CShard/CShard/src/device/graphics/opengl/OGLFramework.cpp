#include "OGLFramework.hpp"

#include <stdexcept>
#include <string>
#include <iostream>

#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

void __stdcall openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << "---------------------opengl-callback-start------------" << std::endl;
	std::cout << "message: " << message << std::endl;
	std::cout << "source: ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		std::cout << "API";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::cout << "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		std::cout << "OTHER";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cout << "SHADER COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cout << "THIRD PARTY";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cout << "WINDOW SYSTEM";
		break;
	}
	std::cout << std::endl;

	std::cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "OTHER";
		break;
	}
	std::cout << std::endl;

	std::cout << "id: " << id << std::endl;
	std::cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "HIGH";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "NOTIFICATION";
		break;
	default:
		std::cout << "UNKNOWN (" << severity <<")";
	}
	std::cout << std::endl;
	std::cout << "---------------------opengl-callback-end--------------" << std::endl;
	if (type == GL_DEBUG_TYPE_ERROR)
	{
		__debugbreak();
	}
}

void OGLFramework::init()
{
    gl_context = SDL_GL_CreateContext(SDLFramework::getInstance()->getWindow());

	//Initialize GLEW
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        throw std::runtime_error("Error initializing GLEW!");
    }

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OGLFramework::loadImGuiBackends()
{
	ImGui_ImplSDL2_InitForOpenGL(SDLFramework::getInstance()->getWindow(), gl_context);
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
