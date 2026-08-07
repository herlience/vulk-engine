#pragma once
#include <vulkan/vulkan.h>
#include "../../AssetHandler/texturehandler.h"

namespace vulkDescriptors {
	VkDescriptorSetLayout create_descriptor_set_layout(VkDevice device, VkDescriptorType layoutdescriptortype);
	VkDescriptorPool create_descriptor_pool(VkDevice device);
	VkDescriptorSet create_descriptor_set(VkDevice device, VkDescriptorSetLayout layout, VkDescriptorPool pool);

	void update_descriptor_set(VkDevice device, Texture tex, VkDescriptorSet descset, uint32_t slotindex);

	void destroy_all_descriptor_components(VkDevice device, VkDescriptorPool pool, VkDescriptorSetLayout layout);
}

