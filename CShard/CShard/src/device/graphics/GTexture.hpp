#pragma once

class GTexture
{
public:
	virtual ~GTexture() = default;
	
	virtual void renderAsBackground() = 0;
	virtual void useTexture() = 0;
};

