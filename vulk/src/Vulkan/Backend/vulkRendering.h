#pragma once

#include <vulkan/vulkan.h>
#include "vulkSwapchain.h"
#include <vector>
#include "vulkBuffer.h"

namespace vulkRendering {
	VkCommandPool createCommandPool(VkDevice device, uint32_t queueFamilyIndex);
	VkCommandBuffer allocateCommandBuffer(VkDevice device, VkCommandPool commandPool);

	// Synchronization things
	VkSemaphore createSemaphore(VkDevice device);
	VkFence createFence(VkDevice device);

    void DrawFrame(
        VkDevice device,
        VkSwapchainKHR swapchain,
        const std::vector<VkImage>& images,
        const std::vector<VkImageView>& imageViews,
        VkFormat swapchainFormat,
        VkExtent2D extent,
        VkPipeline pipeline,
        VkPipelineLayout layout,
        const std::vector<VkCommandBuffer>& commandBuffers,
        VkQueue graphicsQueue,
        VkQueue presentQueue,
        size_t& currentFrame,
        const int MAX_FRAMES_IN_FLIGHT,
        const std::vector<VkSemaphore>& imageAvailableSemaphores,
        const std::vector<VkSemaphore>& renderFinishedSemaphores,
        const std::vector<VkFence>& inFlightFences,
        VkBuffer vertexbuffer,
        const std::vector<Vertex>& vertices,
        PFN_vkCmdSetVertexInputEXT& fnCmdSetVertexInputEXT
        );
}

