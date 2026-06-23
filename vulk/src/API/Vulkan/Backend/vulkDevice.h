#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace vulkDevice {
    inline VkPhysicalDevice vulkPhysicalDevice = VK_NULL_HANDLE;
    inline VkDevice vulkLogicalDevice = VK_NULL_HANDLE;
    inline VkQueue vulkGraphicsQueue = VK_NULL_HANDLE;
    inline VkQueue vulkPresentQueue = VK_NULL_HANDLE;

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    void create(VkInstance instance, VkSurfaceKHR surface);
    void clean();
   
    void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
    void createLogicalDevice(VkSurfaceKHR surface);
    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    VkPhysicalDevice getPhysicalDevice();
    VkDevice getLogicalDevice();
    VkQueue getGraphicsQueue();
    VkQueue getPresentQueue();

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
}

