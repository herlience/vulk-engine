#pragma once
#include <vulkan/vulkan.h>
#include <VkBootstrap/VkBootstrap.h>

namespace vulkanBootstrap {
	inline vkb::Instance vkbInstance;
	inline vkb::Device vkbDevice;

	inline VkInstance vulkInstance = VK_NULL_HANDLE;
	inline VkDevice vulkDevice = VK_NULL_HANDLE;
	
	void buildInstance();
	void selectDevice();

	VkInstance getInstance();
	vkb::Instance getVKBinstance();
	VkDevice getDevice();

	void Destroy();
}
