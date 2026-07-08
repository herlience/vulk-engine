#pragma once

#include <vulkan/vulkan.h>
#include "vulkSwapchain.h"
#include <vector>
#include "vulkBuffer.h"

struct frameComponents {
    VkDevice device;
    VkSwapchainKHR swapchain;
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    VkFormat swapchainFormat;
    VkExtent2D extent;
    VkPipeline pipeline;
    VkPipelineLayout layout;
    std::vector<VkCommandBuffer> commandBuffers;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    size_t currentFrame;
    int MAX_FRAMES_IN_FLIGHT;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    VkBuffer vertexbuffer;
    std::vector<Vertex> vertices;
    PFN_vkCmdSetVertexInputEXT fnCmdSetVertexInputEXT;
};

namespace vulkRendering {
	VkCommandPool createCommandPool(VkDevice device, uint32_t queueFamilyIndex);
	VkCommandBuffer allocateCommandBuffer(VkDevice device, VkCommandPool commandPool);

	// Synchronization things
	VkSemaphore createSemaphore(VkDevice device);
	VkFence createFence(VkDevice device);

    void DrawFrame(
        frameComponents& framecomp,
        VkSwapchainKHR swapchain,
        const std::vector<VkImage>& images,
        VkFormat swapchainFormat,
        VkExtent2D extent
        );
}

