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

struct AllocatedImage {
    VkImage image;
    VmaAllocation allocation;
    VmaAllocationInfo info;
    VkFormat imageFormat;
    VkExtent3D imageExtent;
};

struct VulkComponents {
    VkInstance instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugmessenger = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkPhysicalDevice physicaldevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    SwapchainVariables swapchainvariables;
    std::vector<VkImageView> imageViews;
    VkPipeline pipeline = VK_NULL_HANDLE;
    VkPipelineLayout layout = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptorsetlayout;
    VkDescriptorPool descriptorpool;
    std::vector<VkCommandBuffer> commandBuffers;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    size_t currentFrame;
    int MAX_FRAMES_IN_FLIGHT;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    VkBuffer vertexbuffer = VK_NULL_HANDLE;
    VkCommandPool commandpool = VK_NULL_HANDLE;
    VkDeviceMemory vertexbuffermemory = VK_NULL_HANDLE;
    PFN_vkCmdSetVertexInputEXT fnCmdSetVertexInputEXT = VK_NULL_HANDLE;
    VmaAllocator allocator;
    AllocatedImage depthimage;
    VkImageView depthimageview = VK_NULL_HANDLE;
    VkExtent3D threedextent;
    VkSampler defaulttexturesampler = VK_NULL_HANDLE;

    void threedextenttanimlama() {
        threedextent.height = swapchainvariables.extent.height;
        threedextent.width = swapchainvariables.extent.width;
        threedextent.depth = 1;
    }
};

