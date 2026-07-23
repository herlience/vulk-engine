#version 450
#extension GL_EXT_buffer_reference : require
#extension GL_EXT_scalar_block_layout : require

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec3 outNormal; 

struct Vertex {
    vec3 position;
    float uv_x;
    vec3 normal;
    float uv_y;
    vec4 color;
}; 

layout(buffer_reference, scalar) readonly buffer VertexBuffer{ 
    Vertex vertices[];
};


layout( push_constant ) uniform constants
{   
    mat4 render_matrix;
    VertexBuffer vertexBuffer;
} PushConstants;

void main() 
{   
    
    Vertex v = PushConstants.vertexBuffer.vertices[gl_VertexIndex];

    
    gl_Position = PushConstants.render_matrix * vec4(v.position, 1.0f);
    outColor = v.color.xyz;
    outUV.x = v.uv_x;
    outUV.y = v.uv_y;
    
    
    outNormal = mat3(PushConstants.render_matrix) * v.normal;
}