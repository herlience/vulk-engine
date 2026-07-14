#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Render/RendererTypes.h"

namespace GameObject
{
    glm::vec3 translation{ 0.0f };
    glm::vec3 rotation{ 0.0f };
    glm::vec3 scale{ 1.0f };

    VkBuffer indexBuffer;
    uint32_t indexCount;
    VkDeviceAddress vertexBufferAddress;

    glm::mat4 getModelMatrix();
    RenderObject getRenderState();
}

