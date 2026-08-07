#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/include/glfw3.h>
#include "vulkWindow.h"
#include "vulkDevice.h"
#include <vector>
#include "../vulkTypes.h"
#include "vulkBuffer.h"


namespace vulkSwapchain {

    SwapchainVariables create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice logicalDevice, GLFWwindow* window, VkSurfaceKHR surface, QueueFamilyIndices indices);
    std::vector<VkImageView> createImageViews(VkDevice logicalDevice, const std::vector<VkImage>& swapchainImages, VkFormat vulkSwapchainImageFormat);
    void recreateswapchain(VulkComponents& vulkcomp, GLFWwindow* window, QueueFamilyIndices indices);
    
    void clean(VulkComponents& vulkcomp);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

}