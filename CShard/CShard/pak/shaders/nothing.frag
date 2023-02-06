#version 450

precision highp float;

layout(location = 0) out vec4 fragmentColor;
layout(binding = 1) uniform sampler2D render;

in vec2 texCoord;

void main()
{
	fragmentColor = texture(render, texCoord);
}