#version 450

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

layout (location = 0) out vec4 outFragColor;

void main() 
{
    vec3 N = normalize(inNormal);
    
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.7));
    
    float diff = max(dot(N, lightDir), 0.0);
    
    float ambient = 0.15;
    
    float lightIntensity = diff + ambient;
    
    vec3 baseColor = vec3(0.7, 0.7, 0.7);
    
    vec3 finalColor = baseColor * lightIntensity;
    
    outFragColor = vec4(finalColor, 1.0);
}