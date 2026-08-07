#include "vulkDescriptors.h"
#include "../vulkTypes.h"

namespace vulkDescriptors {
	VkDescriptorSetLayout create_descriptor_set_layout(VkDevice device) {
		VkDescriptorSetLayoutBinding layoutbinding{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = 10000,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
			.pImmutableSamplers = nullptr
		};

		VkDescriptorBindingFlags bindingFlags = VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT | VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT; 

		VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo{};
		bindingFlagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
		bindingFlagsInfo.bindingCount = 1;
		bindingFlagsInfo.pBindingFlags = &bindingFlags;

		VkDescriptorSetLayoutCreateInfo layoutinfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext = &bindingFlagsInfo,
			.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT,
			.bindingCount = 1,
			.pBindings = &layoutbinding
		};

		VkDescriptorSetLayout desclayout;
		VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutinfo, nullptr, &desclayout));

		return desclayout;
	}

	VkDescriptorPool create_descriptor_pool(VkDevice device) {
		uint32_t desccount = 10000;
		uint32_t maxsets = 1;

		VkDescriptorPoolSize poolsize{
			.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = desccount
		};

		VkDescriptorPoolCreateInfo poolcreateinfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT,
			.maxSets = maxsets,
			.poolSizeCount = 1,
			.pPoolSizes = &poolsize
		};

		VkDescriptorPool descpool;
		VK_CHECK(vkCreateDescriptorPool(device, &poolcreateinfo, nullptr, &descpool));

		return descpool;
	}

	VkDescriptorSet create_descriptor_set(VkDevice device, VkDescriptorSetLayout layout, VkDescriptorPool pool) {
		uint32_t variablesdesccount = 10000;

		VkDescriptorSetVariableDescriptorCountAllocateInfo variableCountInfo{};
		variableCountInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
		variableCountInfo.descriptorSetCount = 1;
		variableCountInfo.pDescriptorCounts = &variablesdesccount;
		
		VkDescriptorSetAllocateInfo allocinfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = &variableCountInfo,
			.descriptorPool = pool,
			.descriptorSetCount = 1,
			.pSetLayouts = &layout
		};

		VkDescriptorSet descset;
		VK_CHECK(vkAllocateDescriptorSets(device, &allocinfo, &descset));

		return descset;
	}

	void update_descriptor_set(VkDevice device, Texture tex, VkDescriptorSet descset, uint32_t slotindex) {
		VkDescriptorImageInfo imageinfo{
			.sampler = tex.sampler,
			.imageView = tex.imageView,
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		};

		VkWriteDescriptorSet descwrite{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.pNext = nullptr,
			.dstSet = descset,
			.dstBinding = 0,
			.dstArrayElement = slotindex,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.pImageInfo = &imageinfo,
			.pBufferInfo = nullptr,
			.pTexelBufferView = nullptr
		};

		vkUpdateDescriptorSets(device, 1, &descwrite, 0, nullptr);
	}

	void destroy_all_descriptor_components(VkDevice device, VkDescriptorPool pool, VkDescriptorSetLayout layout) {
		vkDestroyDescriptorPool(device, pool, nullptr);
		vkDestroyDescriptorSetLayout(device, layout, nullptr);
	}
}