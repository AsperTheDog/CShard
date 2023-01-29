#pragma once

#include <cstdint>
#include <vec3.hpp>

#include "../GShadowMap.hpp"
#include "OGLTexture.hpp"

class OGLShadowMap final : public GShadowMap
{
public:
	explicit OGLShadowMap(uint32_t size);

    void render(unsigned index, glm::vec3 lightPos) override;

private:
	OGLCubeTexture cubeTex;
	uint32_t depthFBO = 0;

	struct CameraDirection
	{
	    GLenum face;
	    glm::vec3 target;
	    glm::vec3 up;
	};

	inline static CameraDirection camDirections[] =
	{
	    { GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
	    { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
	    { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
	    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
	    { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
	    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
	}; 
};

