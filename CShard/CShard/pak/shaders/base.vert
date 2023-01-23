#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec3 out_pos;
layout(location = 1) out vec2 out_texture;
layout(location = 2) out vec3 out_normal;
 
struct Camera {
    mat4 vMatrix;
    mat4 pMatrix;
};

struct Model
{
    mat4 invMat;
    mat4 mat;
};

uniform Model model;
uniform Camera camMats;

void main() {
    vec4 worldspace = model.mat * vec4(position, 1.0f);
    gl_Position = camMats.pMatrix * camMats.vMatrix * worldspace;
    out_texture = texCoords;
    out_normal = (model.invMat * vec4(normal, 1.0)).xyz;
    out_pos = worldspace.xyz;
}