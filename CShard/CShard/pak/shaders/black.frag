#version 450

precision highp float;

layout(location = 0) out vec4 fragmentColor;
layout(binding = 1) uniform sampler2D render;

in vec2 texCoord;

layout(location = 0) uniform float mult;

void main()
{
	fragmentColor = vec4(texture(render, texCoord).rgb * mult, 1.0);
}