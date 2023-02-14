#pragma once
#include <fstream>

#include "imgui.h"
#include "Shader.hpp"
#include "../../engine/Config.hpp"
#include "FrameBuffer.hpp"
#include "Mesh.hpp"
#include "PostEffectTypes.hpp"

class PostEffect
{
protected:
	~PostEffect() = default;

public:
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
	virtual const char* getName() = 0;

	virtual void serialize(std::ofstream& wf)
	{
		wf.write((char*) &doRender, sizeof(doRender));
	}
	virtual void deserialize(std::ifstream& wf)
	{
		wf.read((char*) &doRender, sizeof(doRender));
	}

	virtual PostType getType() = 0;

	bool doRender = false;
};

struct FilmGrain final : PostEffect
{
	static void commitShader()
	{
		shader.commit(POST_VERTEX_LOCATION, GRAIN_FRAGMENT_LOCATION);
	}

	void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth) override
	{
		glUseProgram(shader.id);
		glUniform1f(glGetUniformLocation(shader.id, "randomSeed"), nextNum);
		glUniform1f(glGetUniformLocation(shader.id, "grainIntensity"), intensity);
		nextNum += 0.01f;
		PostEffect::render(from, to, depth);
	}

	void showImGuiWindow(uint32_t id) override
	{
		std::string uniqueCode = std::to_string(id);
		ImGui::Checkbox(("Active##Grain" + uniqueCode).c_str(), &doRender);
		ImGui::Separator();
		ImGui::DragFloat(("Intensity##Grain" + uniqueCode).c_str(), &intensity, 0.01f, 0.f, 1.f);
	}

	const char* getName() override
	{
		return "Film Grain Effect";
	}

	void serialize(std::ofstream& wf) override
	{
		PostType type = POST_FILMGRAIN;
		wf.write((char*) &type, sizeof(type));
		wf.write((char*) &intensity, sizeof(intensity));
		PostEffect::serialize(wf);
	}

	void deserialize(std::ifstream& wf) override
	{
		wf.read((char*) &intensity, sizeof(intensity));
		PostEffect::deserialize(wf);
	}

	PostType getType() override
	{
		return POST_FILMGRAIN;
	}

	float nextNum = 0.01f;
	float intensity = 0.1f;

private:
	inline static Shader shader{};
};

struct Atmospheric final : PostEffect
{
	static void commitShader()
	{
		shader.commit(POST_VERTEX_LOCATION, ATMOS_FRAGMENT_LOCATION);
	}

	void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth) override
	{
		glUseProgram(shader.id);
		glUniform1f(glGetUniformLocation(shader.id, "density"), density);
		glUniform3fv(glGetUniformLocation(shader.id, "fogColor"), 1, &color.x);
		PostEffect::render(from, to, depth);
	}

	void showImGuiWindow(uint32_t id) override
	{
		std::string uniqueCode = std::to_string(id);
		ImGui::Checkbox(("Active##Atmos" + uniqueCode).c_str(), &doRender);
		ImGui::Separator();
		ImGui::ColorPicker3(("Color##Fog" + uniqueCode).c_str(), &color.x);
		ImGui::DragFloat(("Density##Fog" + uniqueCode).c_str(), &density, 0.0001f, 0.f, 1.f);
	}

	const char* getName() override
	{
		return "Atmospheric Fog Effect";
	}

	void serialize(std::ofstream& wf) override
	{
		PostType type = POST_ATMOSPHERICFOG;
		wf.write((char*) &type, sizeof(type));
		wf.write((char*) &density, sizeof(density));
		wf.write((char*) &color, sizeof(color));
		PostEffect::serialize(wf);
	}

	void deserialize(std::ifstream& wf) override
	{
		wf.read((char*) &density, sizeof(density));
		wf.read((char*) &color, sizeof(color));
		PostEffect::deserialize(wf);
	}

	PostType getType() override
	{
		return POST_ATMOSPHERICFOG;
	}

	float density = 0.f;
	glm::vec3 color{1.f};
private:
	inline static Shader shader{};
};

struct BlackFade final : PostEffect
{
	static void commitShader()
	{
		shader.commit(POST_VERTEX_LOCATION, BLACK_FRAGMENT_LOCATION);
	}

	void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth) override
	{
		glUseProgram(shader.id);
		glUniform1f(glGetUniformLocation(shader.id, "mult"), mult);
		PostEffect::render(from, to, depth);
	}

	void showImGuiWindow(uint32_t id) override
	{
		std::string uniqueCode = std::to_string(id);
		ImGui::Checkbox(("Active##Black" + uniqueCode).c_str(), &doRender);
		ImGui::Separator();
		ImGui::DragFloat(("Intensity##Black" + uniqueCode).c_str(), &mult, 0.01f, 0.f, 1.f);
	}

	const char* getName() override
	{
		return "Dim effect";
	}

	void serialize(std::ofstream& wf) override
	{
		PostType type = POST_FADETOBLACK;
		wf.write((char*) &type, sizeof(type));
		wf.write((char*) &mult, sizeof(mult));
		PostEffect::serialize(wf);
	}

	void deserialize(std::ifstream& wf) override
	{
		wf.read((char*) &mult, sizeof(mult));
		PostEffect::deserialize(wf);
	}

	PostType getType() override
	{
		return POST_FADETOBLACK;
	}

	float mult = 1.f;

private:
	inline static Shader shader{};
};

struct DepthEffect final : PostEffect
{
	static void commitShader()
	{
		shader.commit(POST_VERTEX_LOCATION, DEPTH_FRAGMENT_LOCATION);
	}

	void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth) override
	{
		glUseProgram(shader.id);
		PostEffect::render(from, to, depth);
	}

	void showImGuiWindow(uint32_t id) override
	{
		std::string uniqueCode = std::to_string(id);
		ImGui::Checkbox(("Active##Depth" + uniqueCode).c_str(), &doRender);
	}

	const char* getName() override
	{
		return "Depth Buffer effect";
	}

	void serialize(std::ofstream& wf) override
	{
		PostType type = POST_DEPTHBUFFER;
		wf.write((char*) &type, sizeof(type));
		PostEffect::serialize(wf);
	}

	PostType getType() override
	{
		return POST_DEPTHBUFFER;
	}

private:
	inline static Shader shader{};
};

struct Pixelate final : PostEffect
{
	static void commitShader()
	{
		shader.commit(POST_VERTEX_LOCATION, PIXELATE_FRAGMENT_LOCATION);
	}

	void render(FrameBuffer* from, FrameBuffer* to, EmptyTexture* depth) override
	{
		glUseProgram(shader.id);
		glUniform1i(0, subsample);
		PostEffect::render(from, to, depth);
	}

	void showImGuiWindow(uint32_t id) override
	{
		std::string uniqueCode = std::to_string(id);
		ImGui::Checkbox(("Active##Pixel" + uniqueCode).c_str(), &doRender);
		ImGui::Separator();
		ImGui::DragInt(("subsample##Pixel" + uniqueCode).c_str(), &subsample, 1, 1, 30);
	}

	const char* getName() override
	{
		return "Pixelate effect";
	}

	void serialize(std::ofstream& wf) override
	{
		PostType type = POST_PIXELATE;
		wf.write((char*) &type, sizeof(type));
		wf.write((char*) &subsample, sizeof(subsample));
		PostEffect::serialize(wf);
	}

	void deserialize(std::ifstream& wf) override
	{
		wf.read((char*) &subsample, sizeof(subsample));
		PostEffect::deserialize(wf);
	}

	PostType getType() override
	{
		return POST_PIXELATE;
	}

	int subsample = 1;

private:
	inline static Shader shader{};
};
