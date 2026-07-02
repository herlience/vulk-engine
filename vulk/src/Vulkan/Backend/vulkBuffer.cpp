#include "vulkBuffer.h"

namespace vulkBuffer {
	VkBuffer createVertexBuffer(VkDevice device, VkDeviceMemory& vertexbuffermemory, VkBufferCreateInfo bufferInfo, VkPhysicalDevice physicaldevice) {
		VkBuffer vertexbuffer = VK_NULL_HANDLE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertexbuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, vertexbuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, physicaldevice);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &vertexbuffermemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        return vertexbuffer;
	}

    void MapMemory(VkDevice device, VkDeviceMemory& buffermemory, const std::vector<Vertex>& vertices, VkBufferCreateInfo bufferInfo) {
        void* data;
        vkMapMemory(device, buffermemory, 0, bufferInfo.size, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferInfo.size);
        vkUnmapMemory(device, buffermemory);
    }

    void bindMemory(VkDevice device, VkBuffer vertexbuffer, VkDeviceMemory& vertexbuffermemory) {
        vkBindBufferMemory(device, vertexbuffer, vertexbuffermemory, 0);
    }

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");

    }
}