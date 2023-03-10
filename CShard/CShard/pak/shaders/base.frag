#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

layout(binding = 0) uniform sampler2D tex;

layout (location = 0) out vec4 outColor;

struct PointLight { 
    float constant;
    float linear;
    float quadratic;  

    vec3 position;
    vec3 color;
};  

struct Camera {
    vec3 pos;
    vec3 dir;
};

struct Material {
    vec3 albedo;
    float emission;
    float shininess;
}; 

uniform PointLight pLights[20];
uniform Camera cam;
uniform Material mat;
uniform uint lightNum;

vec3 CalcPointLight(uint index, vec3 normal, vec3 camDir)
{
    PointLight light = pLights[index];
    vec3 lightDir = normalize(light.position - pos);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float amb = mat.emission;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfV = normalize(lightDir + camDir);
    float spec = pow(max(dot(normal, halfV), 0.0), mat.shininess * 4);

    float distance = length(light.position - pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    vec3 color = texture(tex, texCoords).xyz * mat.albedo;
    
    vec3 ambient = (light.color * amb * color) * attenuation;
    vec3 diffuse = (light.color * diff * color) * attenuation;
    vec3 specular = (light.color * spec * color) * attenuation;

    return (ambient + diffuse + specular);
}

void main() {
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cam.pos - pos);

    float alpha = texture(tex, texCoords).a;
    vec3 result = vec3(0.0, 0.0, 0.0);
    vec4 amb = texture(tex, texCoords) * vec4(mat.albedo, 1.0) * mat.emission;

    for(uint i = 0; i < lightNum; i++)
        result += CalcPointLight(i, norm, viewDir);
    outColor = max(vec4(result, alpha), amb);
}