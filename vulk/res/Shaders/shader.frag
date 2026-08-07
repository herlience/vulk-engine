#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

layout (location = 3) flat in uint inTextureIndex; 

layout (location = 0) out vec4 outFragColor;

layout (set = 0, binding = 0) uniform sampler2D globalTextures[];

void main() 
{
    
    vec3 N = normalize(inNormal);
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.7));
    
    float diff = max(dot(N, lightDir), 0.0);
    float ambient = 0.15;
    float lightIntensity = diff + ambient;
    
    uint index = nonuniformEXT(inTextureIndex);
    vec4 texColor = texture(globalTextures[index], inUV);
    
    vec3 finalColor = texColor.rgb * lightIntensity;
    
    outFragColor = vec4(finalColor, texColor.a);
}