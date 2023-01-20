#version 450

#define PI 3.14159265359

precision highp float;

layout(location = 0) out vec4 fragmentColor;
layout(binding = 2) uniform sampler2D background;

in vec2 texCoord;

uniform mat4 invPV;
uniform vec3 camPos;

void main()
{
	vec4 worldPos = invPV * vec4(texCoord * 2.0 - 1.0, 1.0, 1.0);
	worldPos = (1.0 / worldPos.w) * worldPos;

	vec3 dir = normalize(worldPos.xyz - camPos);

	float theta = acos(max(-1.0f, min(1.0f, dir.y)));
	float phi = atan(dir.z, dir.x);
	if(phi < 0.0f)
	{
		phi = phi + 2.0f * PI;
	}

	vec2 lookup = vec2(phi / (2.0 * PI), 1 - theta / PI);
	fragmentColor = texture(background, lookup);
}