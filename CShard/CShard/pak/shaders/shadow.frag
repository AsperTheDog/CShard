#version 450

layout(location = 0) out vec4 fragmentColor;

#define farPlane 100.0
        
void main()
{
    fragmentColor = vec4(1.0, 1.0, 1.0, 1.0);
}