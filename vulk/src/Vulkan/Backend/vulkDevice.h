#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include "../vulkTypes.h"

namespace vulkDevice {

    void clean(VkDevice vulkLogicalDevice, VkPhysicalDevice vulkPhysicalDevice);
   
    VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
    VkDevice createLogicalDevice(VkSurfaceKHR surface, VkPhysicalDevice vulkPhysicalDevice, QueueFamilyIndices indices, PFN_vkCmdSetVertexInputEXT& fnCmdSetVertexInputEXT);

    VkQueue createGraphicsQueue(VkDevice vulkLogicalDevice, QueueFamilyIndices indices);
    VkQueue createPresentQueue(VkDevice vulkLogicalDevice, QueueFamilyIndices indices);

    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME
    };
}

