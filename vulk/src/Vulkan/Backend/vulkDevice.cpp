#define GLFW_INCLUDE_VULKAN

#ifdef _WIN32
extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

#include "vulkDevice.h"
#include <GLFW/include/glfw3.h>
#include <iostream>
#include <set>
#include <string>

namespace vulkDevice {

    VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
        VkPhysicalDevice vulkPhysicalDevice = VK_NULL_HANDLE;
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("We have a problem for finding the available GPU for Vulkan!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        VkPhysicalDevice selectedDevice = VK_NULL_HANDLE;

        for (const auto& device : devices) {
            if (isDeviceSuitable(device, surface)) {
                VkPhysicalDeviceProperties props;
                vkGetPhysicalDeviceProperties(device, &props);

                if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                    selectedDevice = device;
                    std::cout << "[GPU Secildi]: " << props.deviceName << " (Discrete GPU)" << std::endl;
                    break;
                }
            }
        }

        if (selectedDevice == VK_NULL_HANDLE) {
            for (const auto& device : devices) {
                if (isDeviceSuitable(device, surface)) {
                    selectedDevice = device;
                    VkPhysicalDeviceProperties props;
                    vkGetPhysicalDeviceProperties(selectedDevice, &props);
                    std::cout << "[GPU Secildi]: " << props.deviceName << " (Integrated/Fallback GPU)" << std::endl;
                    break;
                }
            }
        }

        if (selectedDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("Gerekli ozellikleri karsilayan bir GPU bulunamadi!");
        }

        return selectedDevice;
    }

    VkDevice createLogicalDevice(VkSurfaceKHR surface, VkPhysicalDevice vulkPhysicalDevice, QueueFamilyIndices indices, PFN_vkCmdSetVertexInputEXT& fnCmdSetVertexInputEXT) {
        VkDevice vulkLogicalDevice = VK_NULL_HANDLE;

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

        VkPhysicalDeviceVulkan12Features features12 = {};
        features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
        features12.bufferDeviceAddress = VK_TRUE;
        features12.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
        features12.descriptorBindingPartiallyBound = VK_TRUE;
        features12.descriptorBindingVariableDescriptorCount = VK_TRUE;
        features12.runtimeDescriptorArray = VK_TRUE;
        features12.pNext = nullptr;

        VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT dynamicVertexFeatures{};
        dynamicVertexFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VERTEX_INPUT_DYNAMIC_STATE_FEATURES_EXT;
        dynamicVertexFeatures.vertexInputDynamicState = VK_TRUE;
        dynamicVertexFeatures.pNext = &features12; 

        VkPhysicalDeviceSynchronization2Features sync2Features{};
        sync2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
        sync2Features.synchronization2 = VK_TRUE;
        sync2Features.pNext = &dynamicVertexFeatures;

        VkPhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeatures{};
        dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES;
        dynamicRenderingFeatures.dynamicRendering = VK_TRUE;
        dynamicRenderingFeatures.pNext = &sync2Features;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.pNext = &dynamicRenderingFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        createInfo.enabledLayerCount = 0;

        if (vkCreateDevice(vulkPhysicalDevice, &createInfo, nullptr, &vulkLogicalDevice) != VK_SUCCESS) {
            throw std::runtime_error("We have a problem for creating logical device!");
        }

        fnCmdSetVertexInputEXT = (PFN_vkCmdSetVertexInputEXT)vkGetDeviceProcAddr(vulkLogicalDevice, "vkCmdSetVertexInputEXT");

        if (fnCmdSetVertexInputEXT == nullptr) {
            std::cout << "We have a problem on VertexInputEXT" << std::endl;
        }

        return vulkLogicalDevice;
    }

    VkQueue createGraphicsQueue(VkDevice vulkLogicalDevice, QueueFamilyIndices indices) {
        VkQueue vulkGraphicsQueue = VK_NULL_HANDLE;
        vkGetDeviceQueue(vulkLogicalDevice, indices.graphicsFamily.value(), 0, &vulkGraphicsQueue);

        return vulkGraphicsQueue;
    }

    VkQueue createPresentQueue(VkDevice vulkLogicalDevice, QueueFamilyIndices indices) {
        VkQueue vulkPresentQueue = VK_NULL_HANDLE;
        vkGetDeviceQueue(vulkLogicalDevice, indices.presentFamily.value(), 0, &vulkPresentQueue);

        return vulkPresentQueue;
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

    void clean(VkDevice vulkLogicalDevice, VkPhysicalDevice vulkPhysicalDevice) {
        if (vulkLogicalDevice != VK_NULL_HANDLE) {
            vkDestroyDevice(vulkLogicalDevice, nullptr);
            vulkLogicalDevice = VK_NULL_HANDLE;
        }
        vulkPhysicalDevice = VK_NULL_HANDLE;
    }
}