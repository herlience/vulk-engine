#include "vulkRendering.h"
#include "vulkBuffer.h"
#include <iostream>
#include <vector>
#include <span>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vulkRendering {
	VkCommandPool createCommandPool(VkDevice device, uint32_t queueFamilyIndex) {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndex;

        VkCommandPool commandPool;
        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            std::cout << "We have a problem on Command Buffer!" << std::endl;;
            return VK_NULL_HANDLE;
        }
        return commandPool;
	}

    VkCommandBuffer allocateCommandBuffer(VkDevice device, VkCommandPool commandPool) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;

        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
            std::cout << "We have a problem in allocating command buffer!" << std::endl;
            return VK_NULL_HANDLE;
        }
        return commandBuffer;
    }

    VkSemaphore createSemaphore(VkDevice device) {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore semaphore;
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
            std::cout << "We have a problem on Semaphore" << std::endl;
            return VK_NULL_HANDLE;
        }
        return semaphore;
    }

    VkFence createFence(VkDevice device) {
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VkFence fence;
        if (vkCreateFence(device, &fenceInfo, nullptr, &fence) != VK_SUCCESS) {
            std::cout << "We have a problem on Fence!" << std::endl;
            return VK_NULL_HANDLE;
        }
        return fence;
    }

}