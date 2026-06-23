#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/include/glfw3.h>
#include "vulkWindow.h"

#include <vector>


namespace vulkSwapchain {
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    void create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice logicalDevice, GLFWwindow* window);
    void createImageViews(VkDevice logicalDevice);
    void clean(VkDevice logicalDevice);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

    VkSwapchainKHR getSwapchain();
    VkFormat getSwapchainImageFormat();
    VkExtent2D getSwapchainExtent();
    const std::vector<VkImageView>& getSwapchainImageViews();
}