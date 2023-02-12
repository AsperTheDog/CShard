#version 450

precision highp float;

layout(location = 0) out vec4 fragmentColor;
layout(binding = 1) uniform sampler2D render;
layout(binding = 2) uniform sampler2D depth;

in vec2 texCoord;

layout(location = 0) uniform vec3 fogColor;
layout(location = 1) uniform float density;

#define FAR 100000.0
#define NEAR 0.1

float LinearDepth(float z) {
    return 1.0 / ((1 - FAR / NEAR) * z + (FAR / NEAR));
}

float fogExp2(float dist, float dens){
	float factor = (dist * (dens / sqrt(log(2.0f))));
	return exp2(-factor * factor);
}

void main()
{
	vec3 albedo = texture(render, texCoord).rgb;
	float depthVal = texture(depth, texCoord).r;
	depthVal = LinearDepth(depthVal);
    float viewDistance = depthVal * FAR - NEAR;
	float contrastFactor = 1 - clamp(fogExp2(viewDistance, density), 0.0f, 1.0f);

	if (depthVal < 0.99999f)
        albedo = mix(albedo.rgb, fogColor, contrastFactor).rgb;

	fragmentColor = vec4(albedo, 1.0f);

}