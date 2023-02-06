#pragma once
#include <cstdint>
#include <stdexcept>

#include "Texture.hpp"

class FrameBuffer
{
public:
	void commit()
	{
		color.commit(COLOR, 1920, 1080);
		depth.commit(DEPTH, 1920, 1080);
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color.texture, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth.texture, 0);
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE)
		{
			throw std::runtime_error(R"(BaseFramebuffer could not be completed)");
		}
	}

	void resize(uint32_t width, uint32_t heigth)
	{
		color.resize(width, heigth, nullptr);
		depth.resize(width, heigth, nullptr);
	}

	void bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	uint32_t id{};
	EmptyTexture color{};
	EmptyTexture depth{};
};
