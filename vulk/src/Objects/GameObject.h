#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Render/RendererTypes.h"

class GameObject
{
public:
    glm::vec3 translation{0.0f, 0.0f, 0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};

    uint32_t renderObjectIndex = 0;

    VkBuffer indexBuffer;
    uint32_t indexCount;
    VkDeviceAddress vertexBufferAddress;

    glm::mat4 getModelMatrix() const;
    RenderObject getRenderState();
};

