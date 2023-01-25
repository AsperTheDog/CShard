#include "GMesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include <tiny_obj_loader.h>

GMesh::GMesh(std::string filepath) : name(filepath)
{
	
	tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        throw std::runtime_error(warn + err);
    }

	for (const auto& shape : shapes) {
	    for (const auto& index : shape.mesh.indices) {
	        Vertex vertex{};
			vertex.coords = {
			    attrib.vertices[3 * index.vertex_index + 0],
			    attrib.vertices[3 * index.vertex_index + 1],
			    attrib.vertices[3 * index.vertex_index + 2]
			};
			if (attrib.texcoords.size() <= 2 * index.texcoord_index + 1)
				vertex.texUV = {0, 0};
			else
				vertex.texUV = {
				    attrib.texcoords[2 * index.texcoord_index + 0],
				    attrib.texcoords[2 * index.texcoord_index + 1]
				};
			if (attrib.normals.size() <= 3 * index.normal_index + 2)
				vertex.normal = {0, 0, 0};
			else
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};
	        vertices.push_back(vertex);
	        indices.push_back((uint32_t)indices.size());
	    }
	}
}
