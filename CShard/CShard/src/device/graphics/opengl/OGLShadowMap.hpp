#pragma once

#include <cstdint>

#include "../GShadowMap.hpp"
#include "OGLTexture.hpp"

class OGLShadowMap final : public GShadowMap
{
	explicit OGLShadowMap(uint32_t size);

private:
	OGLCubeTexture cubeTex;
	OGLEmptyTexture depthTex;
	uint32_t depthFBO = 0;
};

