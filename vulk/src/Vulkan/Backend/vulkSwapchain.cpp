#define GLFW_INCLUDE_VULKAN
#include <GLFW/include/glfw3.h>

#include "vulkSwapchain.h"

#include <algorithm>
#include <iostream>
#include <limits>

namespace vulkSwapchain {
    
    SwapchainVariables create(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice logicalDevice, GLFWwindow* window, VkSurfaceKHR surface, QueueFamilyIndices indices) {
        SwapchainVariables variables;
        std::vector<VkImage> swapchainImages;
        VkSwapchainKHR vulkSwapchain;

        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &vulkSwapchain) != VK_SUCCESS) {
            throw std::runtime_error("We have a problem on Swapchain!");
        }

        vkGetSwapchainImagesKHR(logicalDevice, vulkSwapchain, &imageCount, nullptr);
        swapchainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(logicalDevice, vulkSwapchain, &imageCount, swapchainImages.data());

        variables.format = surfaceFormat.format;
        variables.extent = extent;
        variables.swapchain = vulkSwapchain;
        variables.images = swapchainImages;
        variables.imagecount = imageCount;

        return variables;
    }

    std::vector<VkImageView> createImageViews(VkDevice logicalDevice, const std::vector<VkImage>& swapchainImages, VkFormat vulkSwapchainImageFormat) {
        std::vector<VkImageView> swapchainImageViews;

        swapchainImageViews.resize(swapchainImages.size());

        for (size_t i = 0; i < swapchainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapchainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = vulkSwapchainImageFormat;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("We have a problem on Swapchain Image View!");
            }
        }

        return swapchainImageViews;
    }

    void recreateswapchain(VulkComponents& vulkcomp, GLFWwindow* window, QueueFamilyIndices indices) {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(vulkcomp.device);

        for (auto imageView : vulkcomp.imageViews) {
            vkDestroyImageView(vulkcomp.device, imageView, nullptr);
        }
        vulkcomp.imageViews.clear();

        if (vulkcomp.depthimageview != VK_NULL_HANDLE) {
            vulkBuffer::destroy_imageview(vulkcomp.device, vulkcomp.depthimageview);
            vulkcomp.depthimageview = VK_NULL_HANDLE;
        }
        if (vulkcomp.depthimage.image != VK_NULL_HANDLE) {
            vulkBuffer::destroy_image(vulkcomp.depthimage, vulkcomp.allocator);
            vulkcomp.depthimage.image = VK_NULL_HANDLE;
        }

        if (vulkcomp.swapchainvariables.swapchain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(vulkcomp.device, vulkcomp.swapchainvariables.swapchain, nullptr);
            vulkcomp.swapchainvariables.swapchain = VK_NULL_HANDLE;
        }

        vulkcomp.swapchainvariables = create(
            vulkcomp.instance,
            vulkcomp.physicaldevice,
            vulkcomp.device,
            window,
            vulkcomp.surface,
            indices
        );

        vulkcomp.imageViews = createImageViews(
            vulkcomp.device,
            vulkcomp.swapchainvariables.images,
            vulkcomp.swapchainvariables.format
        );

        vulkcomp.threedextenttanimlama();

        vulkcomp.depthimage = vulkBuffer::create_image(
            vulkcomp.threedextent,
            VK_FORMAT_D32_SFLOAT,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            vulkcomp.allocator
        );

        vulkcomp.depthimageview = vulkBuffer::createimageview(
            vulkcomp.depthimage,
            vulkcomp.device,
            VK_IMAGE_ASPECT_DEPTH_BIT
        );
    }

    void clean(VulkComponents& vulkcomp) {
        for (auto imageView : vulkcomp.imageViews) {
            if (imageView != VK_NULL_HANDLE) {
                vkDestroyImageView(vulkcomp.device, imageView, nullptr);
            }
        }
        vulkcomp.imageViews.clear();

        if (vulkcomp.depthimageview != VK_NULL_HANDLE) {
            vkDestroyImageView(vulkcomp.device, vulkcomp.depthimageview, nullptr);
            vulkcomp.depthimageview = VK_NULL_HANDLE;
        }

        vulkBuffer::destroy_image(vulkcomp.depthimage, vulkcomp.allocator);

        if (vulkcomp.swapchainvariables.swapchain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(vulkcomp.device, vulkcomp.swapchainvariables.swapchain, nullptr);
            vulkcomp.swapchainvariables.swapchain = VK_NULL_HANDLE;
        }

        vulkcomp.swapchainvariables.images.clear();
    }

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }
        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

}