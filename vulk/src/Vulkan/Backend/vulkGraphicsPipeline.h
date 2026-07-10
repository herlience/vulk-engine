#pragma once
#include <vulkan/vulkan.h>


namespace vulkGraphicsPipeline {
    VkPipeline CreateGraphicsPipeline(VkDevice device, VkPipelineLayout pipelineLayout, VkShaderModule vertexShader, VkShaderModule fragmentShader, VkFormat swapchainFormat);
    void DestroyPipeline(VkPipeline pipeline, VkDevice device);
    VkPipelineLayout CreatePipelineLayout(VkDevice device);
    void DestroyPipelineLayout(VkPipelineLayout pipelineLayout, VkDevice device);
}
