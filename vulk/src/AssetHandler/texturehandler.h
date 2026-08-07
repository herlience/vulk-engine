#pragma once
#include <vulkan/vulkan.h>
#include <cstdint>
#include "../Vulkan/vulkTypes.h"

struct Texture {
    AllocatedImage image;
    VkDeviceMemory imageMemory = VK_NULL_HANDLE;
    VkExtent3D texextent;
    VkImageView imageView = VK_NULL_HANDLE;
    VkSampler sampler = VK_NULL_HANDLE;
    uint32_t width = 0;
    uint32_t height = 0;
};

namespace texturehandler {
	Texture loadtexturefile(const char* filepath, VulkComponents vulkcomp);
    uint32_t loadtexturetoengine(const char* filepath, VulkComponents vulkcomp);

    void destroy_textures(VulkComponents vulkcomp);
}

