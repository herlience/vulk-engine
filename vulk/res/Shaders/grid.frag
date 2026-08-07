#version 450

layout(location = 0) in vec3 inWorldPos;
layout(location = 0) out vec4 outColor;

layout(std430, push_constant) uniform GridPushConstants {
    mat4 viewProj;   
    vec3 cameraPos;  
} push;

float getGrid(vec2 st, float space) {
    vec2 coord = st / space;
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    float line = min(grid.x, grid.y);
    return 1.0 - min(line, 1.0);
}

void main() {
    vec2 st = inWorldPos.xz;

    float mainGrid = getGrid(st, 1.0);  
    float subGrid  = getGrid(st, 0.2);  

    vec3 gridColor = vec3(0.35); 
    float alpha = 0.0;

    alpha = mix(alpha, 0.15, subGrid);
    alpha = mix(alpha, 0.40, mainGrid);

    
    float dist = length(inWorldPos.xz - push.cameraPos.xz);
    float minDistance = 1.0;
    float maxDistance = 250.0; 
    float fade = 1.0 - smoothstep(minDistance, maxDistance, dist);

    alpha *= fade;
    outColor = vec4(gridColor, alpha);
}