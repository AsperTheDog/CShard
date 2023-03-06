#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normal;

uniform mat4 MVPmat;

void main() {
    gl_Position = MVPmat * vec4(position, 1.0f);
}