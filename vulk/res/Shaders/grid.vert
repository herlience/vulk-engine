#version 450

layout(push_constant) uniform GridPushConstants {
    mat4 viewProj;
    vec3 cameraPos;
} push;

layout(location = 0) out vec3 outWorldPos;

vec3 positions[6] = vec3[](
    vec3(-1.0, 0.0, -1.0),
    vec3( 1.0, 0.0, -1.0),
    vec3(-1.0, 0.0,  1.0),
    vec3( 1.0, 0.0, -1.0),
    vec3( 1.0, 0.0,  1.0),
    vec3(-1.0, 0.0,  1.0)
);

void main() {
    vec3 localPos = positions[gl_VertexIndex];
    vec3 worldPos = localPos * 1000.0;
    worldPos.x += push.cameraPos.x;
    worldPos.z += push.cameraPos.z;

    outWorldPos = worldPos;
    gl_Position = push.viewProj * vec4(worldPos, 1.0);
}