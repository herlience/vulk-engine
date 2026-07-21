#pragma once

#include "../vulkTypes.h"
#include <vulkan/vulkan.h>
#include <VMA/vk_mem_alloc.h>
#include <glm/glm.hpp>
#include "../../Render/RendererTypes.h"
#include <array>
#include <vector>
#include <iostream>
#include <span>

namespace vulkBuffer {
	AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, VmaAllocator _allocator);
	AllocatedImage create_image(VkExtent3D size, VkFormat format, VkImageUsageFlags usage, VmaAllocator allocator);
	VkImageView createdepthimageview(AllocatedImage depthimage, VkDevice device);
	void destroy_buffer(const AllocatedBuffer& buffer, VmaAllocator _allocator);
	void destroy_image(const AllocatedImage& image, VmaAllocator allocator);
	void destroy_imageview(VkDevice device, VkImageView imageview);
}

