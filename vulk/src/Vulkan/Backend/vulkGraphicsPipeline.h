#pragma once
#include <vulkan/vulkan.h>
#include "../vulkTypes.h"

struct VulkComponents;

namespace vulkGraphicsPipeline {
    VkPipeline CreateGraphicsPipeline(
        VkDevice device,
        VkPipelineLayout pipelineLayout,
        VkShaderModule vertexShader,
        VkShaderModule fragmentShader,
        VkFormat swapchainFormat,
        VkBool32 dwe,
        VkBool32 be,
        VkBlendFactor sCBF,
        VkBlendFactor dCBF
    );
    void DestroyPipeline(VkPipeline pipeline, VkDevice device);
    VkPipelineLayout CreatePipelineLayout(const VulkComponents& vulkcomp, VkBool32 descsetlay, uint32_t pushConstantSize);
    void DestroyPipelineLayout(VkPipelineLayout pipelineLayout, VkDevice device);
}
