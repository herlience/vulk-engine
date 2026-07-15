#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "../Vulkan/vulkTypes.h"

struct GPUDrawPushConstants {
    glm::mat4 worldMatrix;
    VkDeviceAddress vertexBuffer;
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
    glm::mat4 modelMatrix;
    uint64_t vertexBufferAddress;
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