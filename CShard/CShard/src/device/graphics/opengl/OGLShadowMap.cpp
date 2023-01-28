#include "OGLShadowMap.hpp"

#include <stdexcept>

#include "OGLTexture.hpp"
#include "../GraphicsHeader.hpp"

OGLShadowMap::OGLShadowMap(uint32_t size)
	: cubeTex{size, size}, depthTex{DEPTH, size, size, LINEAR, LINEAR, CLAMP, CLAMP}
{
	glGenFramebuffers(1, &depthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex.texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error(R"(BaseFramebuffer could not be completed)");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
