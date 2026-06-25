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

    VkSwapchainKHR create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice logicalDevice, GLFWwindow* window, VkSurfaceKHR surface);
    std::vector<VkImageView> createImageViews(VkDevice logicalDevice);
    void clean(VkDevice logicalDevice, VkSwapchainKHR vulkSwapchain, std::vector<VkImageView>& swapchainImageViews);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);


    /*VkFormat getSwapchainImageFormat();
    VkExtent2D getSwapchainExtent();*/

}