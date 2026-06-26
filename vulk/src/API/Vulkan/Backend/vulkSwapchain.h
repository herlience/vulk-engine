#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/include/glfw3.h>
#include "vulkWindow.h"
#include "vulkDevice.h"
#include <vector>


namespace vulkSwapchain {
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct SwapchainVariables {
        VkSwapchainKHR swapchain = VK_NULL_HANDLE;
        std::vector<VkImage> images;
        VkFormat format;
        VkExtent2D extent;
    };

    SwapchainVariables create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice logicalDevice, GLFWwindow* window, VkSurfaceKHR surface, vulkDevice::QueueFamilyIndices indices);
    std::vector<VkImageView> createImageViews(VkDevice logicalDevice, const std::vector<VkImage>& swapchainImages, VkFormat vulkSwapchainImageFormat);
    void clean(VkDevice logicalDevice, VkSwapchainKHR& vulkSwapchain, std::vector<VkImageView>& swapchainImageViews, std::vector<VkImage>& swapchainImages);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);


    /*VkFormat getSwapchainImageFormat();
    VkExtent2D getSwapchainExtent();*/

}