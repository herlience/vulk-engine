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

	VkCommandBuffer begin_single_time_commands(VulkComponents vulkcomp);
	void end_single_time_commands(VulkComponents vulkcomp, VkCommandBuffer commandBuffer);
	void transition_image_layout(VkCommandBuffer cmd, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copy_buffer_to_image(VkCommandBuffer cmd, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void destroy_buffer(const AllocatedBuffer& buffer, VmaAllocator _allocator);
	void destroy_image(const AllocatedImage& image, VmaAllocator allocator);
	void destroy_imageview(VkDevice device, VkImageView imageview);
}

