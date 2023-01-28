#version 450

#define PI 3.14159265359

precision highp float;

layout(location = 0) out vec4 fragmentColor;
layout(binding = 1) uniform sampler2D render;

in vec2 texCoord;

uniform uint current;
uniform float randomSeed;
uniform float mult;

// https://thebookofshaders.com/10/
float random( vec2 p )
{
	vec2 K1 = vec2(
		23.14069263277926, // e^pi (Gelfond's constant)
		2.665144142690225 // 2^sqrt(2) (Gelfond–Schneider constant)
	);
	return fract( cos( dot(p,K1) ) * 12345.6789 );
}

vec3 dithering(vec3 rgbSample)
{
	vec2 uvr = texCoord;
	uvr.y *= random(vec2(uvr.y, randomSeed));
	rgbSample += random(uvr)*0.15;
	return rgbSample;
}

void main()
{
	vec3 color = texture(render, texCoord).rgb;
	switch(current)
	{
	case 0:
		break;
	case 1:
		color = dithering(color);
		break;
	}
	fragmentColor = vec4(color * mult, 1.0);
}