#pragma once
#include <vulkan/vulkan.h>

namespace vulkGraphicsPipeline {
    VkPipeline CreateGraphicsPipeline(VkDevice device, VkPipelineLayout pipelineLayout, VkShaderModule vertexShader, VkShaderModule fragmentShader, VkFormat swapchainFormat);
    void DestroyPipeline(VkPipeline pipeline, VkDevice device);
    VkPipelineLayout CreatePipelineLayout(VkDevice device, VkDescriptorSetLayout descriptorlayout);
    void DestroyPipelineLayout(VkPipelineLayout pipelineLayout, VkDevice device);
}
