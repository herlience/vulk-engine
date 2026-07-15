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
	void destroy_buffer(const AllocatedBuffer& buffer, VmaAllocator _allocator);
}

