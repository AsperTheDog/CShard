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

	GLuint attribLoc = glGetAttribLocation(program, "position");
	glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      reinterpret_cast<GLvoid*>(offsetof(Vertex, coords)));
	glEnableVertexAttribArray(attribLoc);

	attribLoc = glGetAttribLocation(program, "color");
	glVertexAttribPointer(attribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      reinterpret_cast<GLvoid*>(offsetof(Vertex, texUV)));
	glEnableVertexAttribArray(attribLoc);

	attribLoc = glGetAttribLocation(program, "normalIndex");
	glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                      reinterpret_cast<GLvoid*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(attribLoc);
}

OGLMesh::~OGLMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void OGLMesh::render()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, this->indexNum, GL_UNSIGNED_INT, nullptr);
}
