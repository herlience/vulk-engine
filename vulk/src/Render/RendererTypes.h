#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "../Vulkan/vulkTypes.h"

struct alignas(16) GPUDrawPushConstants {
    glm::mat4 worldMatrix;
    VkDeviceAddress vertexBuffer;
    uint32_t textureIndex;
};

struct Vertex {
    glm::vec3 pos;     
    float uv_x;         
    glm::vec3 normal;  
    float uv_y;         
    glm::vec4 color;    
};

struct RenderObject {
    VkBuffer indexBuffer;
    uint32_t indexCount;
    uint32_t firstIndex;  
    uint64_t vertexBufferAddress;
    uint32_t textureIndex;
};

struct GPUMeshBuffers {
    AllocatedBuffer indexBuffer;
    AllocatedBuffer vertexBuffer;
    VkDeviceAddress vertexBufferAddress;
};

struct Mesh {
    GPUMeshBuffers buffers;
    uint32_t indexcount;
    uint32_t vertexcount;
};

