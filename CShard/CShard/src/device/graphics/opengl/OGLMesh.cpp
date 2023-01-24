#include "OGLMesh.hpp"

#include <iostream>

#include "OGLFramework.hpp"

OGLMesh::OGLMesh(const std::string& filepath): GMesh(filepath)
{
	uint32_t program = ((OGLFramework*)GFramework::get())->getBaseShader()->id;

	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	//GLuint attribLoc = glGetAttribLocation(program, "position");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, coords)));
	glEnableVertexAttribArray(0);

	//attribLoc = glGetAttribLocation(program, "texCoords");
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, texUV)));
	glEnableVertexAttribArray(1);

	//attribLoc = glGetAttribLocation(program, "normal");
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(2);
	this->indexNum = indices.size();
}

OGLMesh::~OGLMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void OGLMesh::render(bool culling)
{
	if (!culling) glDisable(GL_CULL_FACE);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, this->indexNum, GL_UNSIGNED_INT, nullptr);
	if (!culling) glEnable(GL_CULL_FACE);
}

BackOGLMesh::BackOGLMesh()
{
	OGLFramework* glFram = ((OGLFramework*)GFramework::get());
	GLuint program = glFram->backgroundShader->id;

	BackVertex vertices[] = {
		{glm::vec2(-1.f, -1.f)},
		{glm::vec2( 1.f, -1.f)},
		{glm::vec2(-1.f,  1.f)},
		{glm::vec2(-1.f,  1.f)},
		{glm::vec2( 1.f, -1.f)},
		{glm::vec2( 1.f,  1.f)}
	};

	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint attribLoc = glGetAttribLocation(program, "position");
	glVertexAttribPointer(attribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(BackVertex), (GLvoid*)(offsetof(BackVertex, coords)));
	glEnableVertexAttribArray(attribLoc);

}

BackOGLMesh::~BackOGLMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void BackOGLMesh::render()
{
	GLboolean depth = false;
	glGetBooleanv(GL_DEPTH_TEST, &depth);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	if (depth) glEnable(GL_DEPTH_TEST);
}
