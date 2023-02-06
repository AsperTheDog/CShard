#version 450

precision highp float;

layout(location = 0) out vec4 fragmentColor;
layout(binding = 1) uniform sampler2D render;
layout(binding = 2) uniform sampler2D depth;

in vec2 texCoord;

void main()
{
	float depthVal = texture(depth, texCoord).r;
	fragmentColor = vec4(depthVal, depthVal, depthVal, 1.0);
}