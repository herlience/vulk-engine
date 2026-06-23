#define GLFW_INCLUDE_VULKAN

#include "vulkDevice.h"
#include <GLFW/include/glfw3.h>
#include <iostream>
#include <set>
#include <string>

namespace vulkDevice {
	void create(VkInstance instance, VkSurfaceKHR surface) {
		pickPhysicalDevice(instance, surface);
		createLogicalDevice(surface);
	}

    void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("We have a problem for finding the available GPU for Vulkan!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (isDeviceSuitable(device, surface)) {
                vulkPhysicalDevice = device;
                break;
            }
        }

        if (vulkPhysicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("We have a problem for GPU suitability!");
        }
    }

    void createLogicalDevice(VkSurfaceKHR surface) {
        QueueFamilyIndices indices = findQueueFamilies(vulkPhysicalDevice, surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        createInfo.enabledLayerCount = 0;

        if (vkCreateDevice(vulkPhysicalDevice, &createInfo, nullptr, &vulkLogicalDevice) != VK_SUCCESS) {
            throw std::runtime_error("We have a problem for creating logical device!");
        }

        vkGetDeviceQueue(vulkLogicalDevice, indices.graphicsFamily.value(), 0, &vulkGraphicsQueue);
        vkGetDeviceQueue(vulkLogicalDevice, indices.presentFamily.value(), 0, &vulkPresentQueue);
    }

    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
        QueueFamilyIndices indices = findQueueFamilies(device, surface);
        bool extensionsSupported = checkDeviceExtensionSupport(device);

        return indices.isComplete() && extensionsSupported;
    }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }
            i++;
        }

        return indices;
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    void clean() {
        if (vulkLogicalDevice != VK_NULL_HANDLE) {
            vkDestroyDevice(vulkLogicalDevice, nullptr);
            vulkLogicalDevice = VK_NULL_HANDLE;
        }
        vulkPhysicalDevice = VK_NULL_HANDLE;
    }

    VkPhysicalDevice getPhysicalDevice() { return vulkPhysicalDevice; }
    VkDevice getLogicalDevice() { return vulkLogicalDevice; }
    VkQueue getGraphicsQueue() { return vulkGraphicsQueue; }
    VkQueue getPresentQueue() { return vulkPresentQueue; }
}