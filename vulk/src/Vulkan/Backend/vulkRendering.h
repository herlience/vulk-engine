#pragma once
#include "vulkSwapchain.h"
#include <vulkan/vulkan.h>
#include <vector>
#include "vulkBuffer.h"

struct VulkComponents {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugmessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicaldevice;
    VkDevice device;
    SwapchainVariables // vulkTypes'a eklenecek düzenlenecek
    std::vector<VkImageView> imageViews;
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
    VkCommandPool commandpool;
    VkDeviceMemory vertexbuffermemory;
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
        VulkComponents& vulkcomp,
        VkSwapchainKHR swapchain,
        const std::vector<VkImage>& images,
        VkFormat swapchainFormat,
        VkExtent2D extent
        );
}

