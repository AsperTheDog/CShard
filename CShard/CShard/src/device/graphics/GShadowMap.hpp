#pragma once


#include "GTexture.hpp"

class GShadowMap
{
public:
	GShadowMap(uint32_t width, uint32_t height);
	virtual ~GShadowMap() = default;

protected:
	GCubeTexture* cubeMap = nullptr;
};

