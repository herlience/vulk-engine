#pragma once

#include "../vulkTypes.h"
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <iostream>

namespace vulkBuffer {
	VkBuffer createVertexBuffer(VkDevice device, VkDeviceMemory& vertexbuffermemory, VkBufferCreateInfo bufferInfo, VkPhysicalDevice physicaldevice);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
	void bindMemory(VkDevice device, VkBuffer vertexbuffer, VkDeviceMemory& vertexbuffermemory);
	void MapMemory(VkDevice device, VkDeviceMemory& buffermemory, const std::vector<Vertex>& vertices, VkBufferCreateInfo bufferInfo);
}

