#include "vulkDescriptors.h"
#include "../vulkTypes.h"

namespace vulkDescriptors {
	VkDescriptorSetLayout create_descriptor_set_layout(VkDevice device, uint32_t binding, VkDescriptorType descriptortype, VkShaderStageFlags stageflag, VkDescriptorSetLayoutCreateFlags flag) {
		VkDescriptorSetLayoutBinding layoutbinding{
			.binding = binding,
			.descriptorType = descriptortype,
			.descriptorCount = 1,
			.stageFlags = stageflag,
			.pImmutableSamplers = nullptr
		};

		VkDescriptorSetLayoutCreateInfo layoutinfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = flag,
			.bindingCount = 1,
			.pBindings = &layoutbinding
		};

		VkDescriptorSetLayout desclayout;
		VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutinfo, nullptr, &desclayout));

		return desclayout;
	}

	VkDescriptorPool create_descriptor_pool(VkDevice device, VkDescriptorType descriptortype, uint32_t descriptorcount, uint32_t maxsets) {
		VkDescriptorPoolSize poolsize{
			.type = descriptortype,
			.descriptorCount = descriptorcount
		};

		VkDescriptorPoolCreateInfo poolcreateinfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.maxSets = maxsets,
			.poolSizeCount = 1,
			.pPoolSizes = &poolsize
		};

		VkDescriptorPool descpool;
		VK_CHECK(vkCreateDescriptorPool(device, &poolcreateinfo, nullptr, &descpool));

		return descpool;
	}

	VkDescriptorSet create_descriptor_set(VkDevice device, VkDescriptorSetLayout layout, VkDescriptorPool pool) {
		VkDescriptorSetAllocateInfo allocinfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = nullptr,
			.descriptorPool = pool,
			.descriptorSetCount = 1,
			.pSetLayouts = &layout
		};

		VkDescriptorSet descset;
		VK_CHECK(vkAllocateDescriptorSets(device, &allocinfo, &descset));

		return descset;
	}

	void update_descriptor_set(VkDevice device, texture tex, VkDescriptorSet descset, VkDescriptorType descriptortype) {
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
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = descriptortype,
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