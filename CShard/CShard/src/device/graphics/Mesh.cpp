#include "Mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include <tiny_obj_loader.h>

#include "GFramework.hpp"
#include "Shader.hpp"

Mesh::Mesh(const std::string& filepath)
{
	commit(filepath);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::commit(const std::string& filepath)
{
	extractData(filepath);
	name = filepath;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	//GLuint attribLoc = glGetAttribLocation(program, "position");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(0);

	//attribLoc = glGetAttribLocation(program, "texCoords");
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, texCoords)));
	glEnableVertexAttribArray(1);

	//attribLoc = glGetAttribLocation(program, "normal");
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, norm)));
	glEnableVertexAttribArray(2);
	this->indexNum = (uint32_t)indices.size();
}

void Mesh::render(bool culling)
{
	if (!culling) glDisable(GL_CULL_FACE);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, this->indexNum, GL_UNSIGNED_INT, nullptr);
	if (!culling) glEnable(GL_CULL_FACE);
}

void Mesh::renderAsSelection()
{
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDisable(GL_CULL_FACE);
	GLboolean depth = false;
	glGetBooleanv(GL_DEPTH_TEST, &depth);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, this->indexNum, GL_UNSIGNED_INT, nullptr);
	if (depth) glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void Mesh::extractData(const std::string& filename)
{
	tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, ("pak/resources/obj/" + filename).c_str()))
        {
	        throw std::runtime_error("Could not load file");
        }
    }

	for (const auto& shape : shapes) {
	    for (const auto& index : shape.mesh.indices) {
	        Vertex vertex{};
			vertex.pos = {
			    attrib.vertices[3 * index.vertex_index + 0],
			    attrib.vertices[3 * index.vertex_index + 1],
			    attrib.vertices[3 * index.vertex_index + 2]
			};
			if (attrib.texcoords.size() <= 2U * (size_t)index.texcoord_index + 1U)
				vertex.texCoords = {0, 0};
			else
				vertex.texCoords = {
				    attrib.texcoords[2 * index.texcoord_index + 0],
				    attrib.texcoords[2 * index.texcoord_index + 1]
				};
			if (attrib.normals.size() <= 3U * (size_t)index.normal_index + 2U)
				vertex.norm = {0, 0, 0};
			else
				vertex.norm = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};
	        vertices.push_back(vertex);
	        indices.push_back((uint32_t)indices.size());
	    }
	}
}

void PostQuad::init()
{
	GLuint program = GFramework::getBackShader()->id;

	PostVertex vertices[] = {
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
	glVertexAttribPointer(attribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(PostVertex), (GLvoid*)(offsetof(PostVertex, pos)));
	glEnableVertexAttribArray(attribLoc);
}

void PostQuad::render()
{
	GLboolean depth = false;
	glGetBooleanv(GL_DEPTH_TEST, &depth);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	if (depth) glEnable(GL_DEPTH_TEST);
}
