#version 450

precision highp float;

layout(location = 0) out vec4 fragmentColor;
layout(binding = 1) uniform sampler2D render;

in vec2 texCoord;

layout(location = 0) uniform float brightness;
layout(location = 1) uniform float saturation;
layout(location = 2) uniform float contrast;
layout(location = 3) uniform float gamma;

float luminance(vec3 color) {
    return dot(color, vec3(0.299, 0.587, 0.114));
}

void main()
{
	vec3 color = texture(render, texCoord).rgb;

	color = contrast * (color - 0.5f) + 0.5f + brightness;
	color = clamp(color, vec3(0.0), vec3(1.0));

	vec3 desat = vec3(luminance(color));
	color = mix(desat, color, saturation);

	color = clamp(color, vec3(0.0), vec3(1.0));

	color = pow(color.rgb, vec3(gamma));
	color = clamp(color, vec3(0.0), vec3(1.0));

	fragmentColor = vec4(color, 1.0);
}