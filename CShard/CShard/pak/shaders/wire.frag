#version 450

layout (location = 0) out vec4 outColor;

layout (location = 1) uniform vec3 wireColor;

void main() {
    outColor = vec4(wireColor, 1.0);
}