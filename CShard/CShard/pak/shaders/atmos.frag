#version 450

precision highp float;

layout(location = 0) out vec4 fragmentColor;
layout(binding = 1) uniform sampler2D render;

layout(location = 0) uniform float startFog;
layout(location = 1) uniform float endFog;
layout(location = 2) uniform vec3 color;

in vec2 texCoord;

void main()
{
	fragmentColor = texture(render, texCoord);
}