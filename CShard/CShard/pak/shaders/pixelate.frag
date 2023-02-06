#version 450

precision highp float;

layout(location = 0) out vec4 fragmentColor;
layout(binding = 1) uniform sampler2D render;

layout(location = 0) uniform int subsample;

in vec2 texCoord;

vec4 textureRect(in sampler2D tex, vec2 rectangleCoord)
{
	return texture(tex, rectangleCoord / textureSize(tex, 0));
}

void main()
{
	fragmentColor = textureRect(render, gl_FragCoord.xy - mod(gl_FragCoord.xy, subsample));
}