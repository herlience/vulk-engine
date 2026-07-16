#pragma once

#define VK_CHECK(x)                                                     \
    do {                                                                \
        VkResult err = x;                                               \
        if (err) {                                                      \
             fmt::print("Detected Vulkan error: {}", string_VkResult(err)); \
            abort();                                                    \
        }                                                               \
    } while (0)

#define FMT_HEADER_ONLY
#include <fmt/core.h>

#include "../Render/RendererTypes.h"
#include "Backend/vulkGraphicsPipeline.h"

#include <vulkan/vulkan.h>
#include <VMA/vk_mem_alloc.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vector>
#include <glm/glm.hpp>
#include <array>
#include <iostream>
#include <string>
#include <optional>

struct SwapchainVariables {
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> images;
    VkFormat format;
    VkExtent2D extent;
    uint32_t imagecount;
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct AllocatedBuffer {
    VkBuffer buffer;
    VmaAllocation allocation;
    VmaAllocationInfo info;
};

struct VulkComponents {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugmessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicaldevice;
    VkDevice device;
    SwapchainVariables swapchainvariables;
    std::vector<VkImageView> imageViews;
    pipevariables pipelinevar;
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
    VmaAllocator allocator;
};

