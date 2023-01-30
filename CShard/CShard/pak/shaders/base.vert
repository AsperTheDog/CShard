#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec3 out_pos;
layout(location = 1) out vec2 out_texture;
layout(location = 2) out vec3 out_normal;

struct Model
{
    mat4 invMat;
    mat4 mat;
};

uniform Model model;
uniform mat4 MVPmat;

void main() {
    gl_Position = MVPmat * vec4(position, 1.0f);
    out_texture = texCoords;
    out_normal = (model.invMat * vec4(normal, 1.0)).xyz;
    out_pos = (model.mat * vec4(position, 1.0f)).xyz;
}