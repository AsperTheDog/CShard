#include "OGLShadowMap.hpp"

#include "OGLTexture.hpp"

OGLShadowMap::OGLShadowMap(uint32_t size) : GShadowMap(size, size)
{
	cubeMap = new OGLCubeTexture(size, size);
}
