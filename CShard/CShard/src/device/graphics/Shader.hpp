#pragma once

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>

#include "GraphicsHeader.hpp"
#include "../window/SDLFramework.hpp"

class Shader
{
public:
	GLuint id{};

	void commit(const std::string& vertex, const std::string& fragment)
	{
		// create shaders then link them into a pipeline
		uint32_t vs = loadShader(GL_VERTEX_SHADER, vertex);
		uint32_t fs = loadShader(GL_FRAGMENT_SHADER, fragment);

		Shader::linkProgram(vs, fs);

		// once the pipeline is done we don't need the shader objects anymore
		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	void linkProgram(uint32_t vs, uint32_t fs)
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
	
	static uint32_t loadShader(GLenum type, const std::string& file)
	{
		GLint success;

		uint32_t shaderID = glCreateShader(type);
		std::string src = loadShaderSrc(file);
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

	static std::string loadShaderSrc(const std::string& file)
	{
		std::string src;
	    std::fstream inFile;

	    inFile.open(file);
	    if (inFile.is_open())
	    {
	        std::string temp;
	        while (std::getline(inFile, temp))
	            src += temp + "\n";
	    }
	    else
	    {
	        SDLFramework::showErrorMessage(
	            "Could not load shader data", 
	            "Make sure the pak folder is not missing in the program folder");
    		throw std::exception("Error loading shader");
	    }
	    inFile.close();
	    return src;
	}
};
