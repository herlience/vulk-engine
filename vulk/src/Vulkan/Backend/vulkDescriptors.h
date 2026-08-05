#pragma once
#include <vulkan/vulkan.h>
#include "../../AssetHandler/texturehandler.h"

namespace vulkDescriptors {
	VkDescriptorSetLayout create_descriptor_set_layout(VkDevice device, uint32_t binding, VkDescriptorType descriptortype, VkShaderStageFlags stageflag = VK_SHADER_STAGE_FRAGMENT_BIT, VkDescriptorSetLayoutCreateFlags flag = 0);
	VkDescriptorPool create_descriptor_pool(VkDevice device, VkDescriptorType descriptortype, uint32_t descriptorcount = 10, uint32_t maxsets = 10);
	VkDescriptorSet create_descriptor_set(VkDevice device, VkDescriptorSetLayout layout, VkDescriptorPool pool);

	void update_descriptor_set(VkDevice device, texture tex, VkDescriptorSet descset, VkDescriptorType descriptortype);

	void destroy_all_descriptor_components(VkDevice device, VkDescriptorPool pool, VkDescriptorSetLayout layout);
}

