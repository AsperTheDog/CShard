#include "OGLShadowMap.hpp"

#include <stdexcept>

#include "OGLFramework.hpp"
#include "OGLTexture.hpp"
#include "../GFramework.hpp"
#include "../../../elements/components/Camera.hpp"
#include "../../../Engine.hpp"

OGLShadowMap::OGLShadowMap(uint32_t size)
	: cubeTex{size, size}
{
	glGenFramebuffers(1, &depthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubeTex.texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error(R"(BaseFramebuffer could not be completed)");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OGLShadowMap::render(uint32_t index, glm::vec3 lightPos)
{
	//GFramework::shadowMapCam.move(lightPos);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	OGLFramework* ogl = (OGLFramework*)GFramework::get();

	ogl->prepareShader(SHADER_BASE);
	glUniform1i(glGetUniformLocation(GFramework::activeShader, ("shadowMaps[" + std::to_string(index) + "]").c_str()), 10 + index);
	ogl->prepareShader(SHADER_SHADOW);

	glUniform3fv(glGetUniformLocation(GFramework::activeShader, "lightPos"), 1, &lightPos.x);
	
	for (auto& camDirection : camDirections)
	{
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, camDirection.face, cubeTex.texture, 0);
		
		glClear(GL_DEPTH_BUFFER_BIT);

		//GFramework::shadowMapCam.lookAt(camDirection.target);
		//GFramework::shadowMapCam.changeWorldUp(camDirection.up);

		//Engine::renderShadow();
		
		ogl->prepareShader(SHADER_BASE);
		glActiveTexture(GL_TEXTURE10 + index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex.texture);
		ogl->prepareShader(SHADER_SHADOW);

		ogl->resizeWindow();
	}
}