#pragma once

#include <vulkan/vulkan.h>

namespace vulkRenderpass {
	VkRenderPass CreateRenderPass(VkFormat swapchainImageFormat, VkDevice device);
}