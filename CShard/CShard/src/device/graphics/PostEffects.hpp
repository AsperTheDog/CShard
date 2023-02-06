#pragma once

#include "imgui.h"
#include "Shader.hpp"
#include "../../engine/Config.hpp"

class PostProcess
{
protected:
	~PostProcess() = default;

public:
	explicit PostProcess(std::string frag)
	{
		shader.commit(POST_VERTEX_LOCATION, frag);
	}

	virtual void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, to->id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, from->color.texture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depth->texture);
		PostQuad::render();
	}

	virtual void showImGuiWindow(uint32_t id) = 0;

	bool doRender = false;
	Shader shader{};
};

struct FilmGrain final : PostProcess
{
	FilmGrain() : PostProcess(GRAIN_FRAGMENT_LOCATION) {}

	void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth) override
	{
		glUseProgram(shader.id);
		glUniform1f(glGetUniformLocation(shader.id, "randomSeed"), nextNum);
		glUniform1f(glGetUniformLocation(shader.id, "grainIntensity"), intensity);
		nextNum += 0.01f;
		PostProcess::render(from, to, depth);
	}

	void showImGuiWindow(uint32_t id) override
	{
		std::string uniqueCode = std::to_string(id);
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Film Grain Effect");
		ImGui::Checkbox(("Active##Grain" + uniqueCode).c_str(), &doRender);
		ImGui::Separator();
		ImGui::DragFloat(("Intensity##Grain" + uniqueCode).c_str(), &intensity, 0.01f, 0.f, 1.f);
		ImGui::Separator();
	}

	float nextNum = 0.01f;
	float intensity = 0.1f;
};

struct DoNothing final : PostProcess
{
	explicit DoNothing() : PostProcess(NOTHING_FRAGMENT_LOCATION)
	{
	}

	void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth) override
	{
		glUseProgram(shader.id);
		PostProcess::render(from, to, depth);
	}

	void showImGuiWindow(uint32_t id) override
	{
		
	}
};

struct Atmospheric final : PostProcess
{
	Atmospheric() : PostProcess(ATMOS_FRAGMENT_LOCATION) {}

	void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth) override
	{
		glUseProgram(shader.id);
		PostProcess::render(from, to, depth);
	}

	void showImGuiWindow(uint32_t id) override
	{
		std::string uniqueCode = std::to_string(id);
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Atmospheric fog effect");
		ImGui::Checkbox("Active##Atmos", &doRender);
		ImGui::Separator();
	}
};

struct BlackFade final : PostProcess
{
	BlackFade() : PostProcess(BLACK_FRAGMENT_LOCATION) {}

	void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth) override
	{
		glUseProgram(shader.id);
		glUniform1f(glGetUniformLocation(shader.id, "mult"), mult);
		PostProcess::render(from, to, depth);
	}

	void showImGuiWindow(uint32_t id) override
	{
		std::string uniqueCode = std::to_string(id);
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Fade to black effect");
		ImGui::Checkbox(("Active##Black" + uniqueCode).c_str(), &doRender);
		ImGui::Separator();
		ImGui::DragFloat(("Intensity##Black" + uniqueCode).c_str(), &mult, 0.01f, 0.f, 1.f);
		ImGui::Separator();
	}

	float mult = 1.f;
};

struct DepthEffect final : PostProcess
{
	explicit DepthEffect() : PostProcess(DEPTH_FRAGMENT_LOCATION)
	{
	}

	void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth) override
	{
		glUseProgram(shader.id);
		PostProcess::render(from, to, depth);
	}

	void showImGuiWindow(uint32_t id) override
	{
		std::string uniqueCode = std::to_string(id);
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Depth effect");
		ImGui::Checkbox(("Active##Depth" + uniqueCode).c_str(), &doRender);
		ImGui::Separator();
	}
};

struct Pixelate final : PostProcess
{
	explicit Pixelate() : PostProcess(PIXELATE_FRAGMENT_LOCATION)
	{
	}

	void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth) override
	{
		glUseProgram(shader.id);
		glUniform1i(0, subsample);
		PostProcess::render(from, to, depth);
	}

	void showImGuiWindow(uint32_t id) override
	{
		std::string uniqueCode = std::to_string(id);
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Pixelate effect");
		ImGui::Checkbox(("Active##Pixel" + uniqueCode).c_str(), &doRender);
		ImGui::Separator();
		ImGui::DragInt(("subsample##Pixel" + uniqueCode).c_str(), &subsample, 1, 1, 30);
		ImGui::Separator();
	}

	int subsample = 1;
};
