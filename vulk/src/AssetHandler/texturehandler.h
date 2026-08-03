#pragma once
#include <vulkan/vulkan.h>
#include <cstdint>
#include "../Vulkan/vulkTypes.h"

struct texture {
    AllocatedImage image;
    VkDeviceMemory imageMemory = VK_NULL_HANDLE;
    VkExtent3D texextent;
    VkImageView imageView = VK_NULL_HANDLE;
    VkSampler sampler = VK_NULL_HANDLE;
    uint32_t width = 0;
    uint32_t height = 0;
};

namespace texturehandler {
	texture loadtexturefile(const char* filepath, VulkComponents vulkcomp);
}

