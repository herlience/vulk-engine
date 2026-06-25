#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace vulkDevice {

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    void clean(VkDevice vulkLogicalDevice, VkPhysicalDevice vulkPhysicalDevice);
   
    VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
    VkDevice createLogicalDevice(VkSurfaceKHR surface, VkPhysicalDevice vulkPhysicalDevice, QueueFamilyIndices indices);

    VkQueue createGraphicsQueue(VkDevice vulkLogicalDevice, QueueFamilyIndices indices);
    VkQueue createPresentQueue(VkDevice vulkLogicalDevice, QueueFamilyIndices indices);

    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
}

