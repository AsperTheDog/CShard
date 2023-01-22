#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec2 out_texture;
layout(location = 1) out vec3 out_normal;

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

void main() {
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(position, 1.0f);
    out_texture = texCoords;
    out_normal = normal;
}