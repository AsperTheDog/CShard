#version 450

layout (location = 0) in vec2 texCoords;
layout (location = 1) in vec3 normal;

layout(binding = 0) uniform sampler2D tex;

layout (location = 0) out vec4 outColor;

void main() {
  outColor = texture(tex, texCoords);
}