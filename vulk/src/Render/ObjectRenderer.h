#pragma once
#include "RendererTypes.h"
#include "../Vulkan/Backend/vulkBuffer.h"
#include <vulkan/vulkan.h>
#include <span>
#include <VMA/vk_mem_alloc.h>

namespace ObjectRenderer
{
	GPUMeshBuffers uploadMesh(
		VkDevice _device, 
		std::span<uint32_t> indices, 
		std::span<Vertex> vertices, 
		VmaAllocator _allocator, 
		VkCommandPool commandPool, 
		VkQueue graphicsQueue
	);
}

