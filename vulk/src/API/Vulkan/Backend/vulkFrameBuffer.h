#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>

namespace vulkFrameBuffer {
	std::vector<VkFramebuffer> create(std::vector<VkImageView>& swapchainImageViews, VkDevice device, VkExtent2D swapchainExtent, VkRenderPass renderPass);
}
