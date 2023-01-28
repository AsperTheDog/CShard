#pragma once
#include <cstdint>

struct FilmGrain
{
	void next()
	{
		nextNum += 0.01f;
	}
	float nextNum = 0.01f;
	float intensity = 0.1f;
	uint32_t id = 1;
};
