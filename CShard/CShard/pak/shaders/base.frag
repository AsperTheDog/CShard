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
    float emission;
    float shininess;
}; 

uniform PointLight pLights[20];
uniform Camera cam;
uniform Material mat;
uniform uint lightNum;
uniform samplerCubeShadow shadowMaps[20];

#define EPSILON 0.1
#define zFar 100000.0
#define zNear 0.1

float getShadow(uint index, vec3 lightDir)
{
    float z_b  = texture(shadowMaps[index], vec4(-lightDir, 1.0));
    float z_n = 2.0 * z_b - 1.0;
    float depth = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
    float dist = length(lightDir);
    
    if (depth >= dist)
        return 0.1;
    else
        return 1.0;
}

vec3 CalcPointLight(uint index, vec3 normal, vec3 camDir)
{
    PointLight light = pLights[index];
    vec3 lightDir = normalize(light.position - pos);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float amb = mat.emission;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfV = normalize(lightDir + camDir);
    float spec = pow(max(dot(normal, halfV), 0.0), mat.shininess * 4);
    
    float shadowContr = getShadow(index, light.position - pos);

    float distance = length(light.position - pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    vec3 color = texture(tex, texCoords).xyz;
    
    vec3 ambient = (light.color * amb * color) * attenuation;
    vec3 diffuse = (light.color * diff * color) * attenuation;
    vec3 specular = (light.color * spec * color) * attenuation;

    return (ambient + shadowContr * (diffuse + specular));
}

void main() {
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(cam.pos - pos);

    float alpha = texture(tex, texCoords).a;
    vec3 result = vec3(0.0, 0.0, 0.0);
    vec4 amb = texture(tex, texCoords) * mat.emission;

    for(uint i = 0; i < lightNum; i++)
        result += CalcPointLight(i, norm, viewDir);
    outColor = max(vec4(result, alpha), amb);
}