#pragma once
#include "vulkSwapchain.h"
#include <vulkan/vulkan.h>
#include <vector>
#include "vulkBuffer.h"
#include "../vulkTypes.h"

namespace vulkRendering {
	VkCommandPool createCommandPool(VkDevice device, uint32_t queueFamilyIndex);
	VkCommandBuffer allocateCommandBuffer(VkDevice device, VkCommandPool commandPool);

	// Synchronization things
	VkSemaphore createSemaphore(VkDevice device);
	VkFence createFence(VkDevice device);

    void DrawFrame(
        VulkComponents& vulkcomp,
        VkSwapchainKHR swapchain,
        const std::vector<VkImage>& images,
        VkFormat swapchainFormat,
        VkExtent2D extent,
        std::span<Vertex> vertices,
        GPUMeshBuffers meshbuffers,
        std::span<uint32_t> recindices
        );
}

