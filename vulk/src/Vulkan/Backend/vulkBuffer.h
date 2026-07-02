#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <iostream>

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;


		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

namespace vulkBuffer {
	VkBuffer createVertexBuffer(VkDevice device, VkDeviceMemory& vertexbuffermemory, VkBufferCreateInfo bufferInfo, VkPhysicalDevice physicaldevice);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
	void bindMemory(VkDevice device, VkBuffer vertexbuffer, VkDeviceMemory& vertexbuffermemory);
	void MapMemory(VkDevice device, VkDeviceMemory& buffermemory, const std::vector<Vertex>& vertices, VkBufferCreateInfo bufferInfo);
}

