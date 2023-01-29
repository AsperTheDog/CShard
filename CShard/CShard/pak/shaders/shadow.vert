#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vexCoords;
layout(location = 2) in vec3 normals;
 
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
    gl_Position = camMats.pMatrix * camMats.vMatrix *  model.mat * vec4(position, 1.0f);
}