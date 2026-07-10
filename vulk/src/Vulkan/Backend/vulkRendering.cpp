#include "vulkRendering.h"
#include "vulkBuffer.h"
#include <iostream>
#include <vector>
#include <span>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vulkRendering {
	VkCommandPool createCommandPool(VkDevice device, uint32_t queueFamilyIndex) {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndex;

        VkCommandPool commandPool;
        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            std::cout << "We have a problem on Command Buffer!" << std::endl;;
            return VK_NULL_HANDLE;
        }
        return commandPool;
	}

    VkCommandBuffer allocateCommandBuffer(VkDevice device, VkCommandPool commandPool) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;

        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
            std::cout << "We have a problem in allocating command buffer!" << std::endl;
            return VK_NULL_HANDLE;
        }
        return commandBuffer;
    }

    VkSemaphore createSemaphore(VkDevice device) {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore semaphore;
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
            std::cout << "We have a problem on Semaphore" << std::endl;
            return VK_NULL_HANDLE;
        }
        return semaphore;
    }

    VkFence createFence(VkDevice device) {
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VkFence fence;
        if (vkCreateFence(device, &fenceInfo, nullptr, &fence) != VK_SUCCESS) {
            std::cout << "We have a problem on Fence!" << std::endl;
            return VK_NULL_HANDLE;
        }
        return fence;
    }

    void DrawFrame(
        VulkComponents& vulkcomp,
        VkSwapchainKHR swapchain,
        const std::vector<VkImage>& images,
        VkFormat swapchainFormat,
        VkExtent2D extent,
        std::span<Vertex> vertices,
        GPUMeshBuffers meshbuffers,
        std::span<uint32_t> recindices
    ) {
        vkWaitForFences(vulkcomp.device, 1, &vulkcomp.inFlightFences[vulkcomp.currentFrame], VK_TRUE, UINT64_MAX);
        vkResetFences(vulkcomp.device, 1, &vulkcomp.inFlightFences[vulkcomp.currentFrame]);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(
            vulkcomp.device,
            swapchain,
            UINT64_MAX,
            vulkcomp.imageAvailableSemaphores[vulkcomp.currentFrame], // 0 veya 1 gidecek
            VK_NULL_HANDLE,
            &imageIndex
        );

        vkResetCommandBuffer(vulkcomp.commandBuffers[vulkcomp.currentFrame], 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(vulkcomp.commandBuffers[vulkcomp.currentFrame], &beginInfo);

        VkImageMemoryBarrier2 imageBarrier{};
        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
        imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        imageBarrier.srcAccessMask = 0;
        imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        imageBarrier.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
        imageBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        imageBarrier.image = images[imageIndex];
        imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageBarrier.subresourceRange.levelCount = 1;
        imageBarrier.subresourceRange.layerCount = 1;

        VkDependencyInfo dependencyInfo{};
        dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
        dependencyInfo.imageMemoryBarrierCount = 1;
        dependencyInfo.pImageMemoryBarriers = &imageBarrier;

        vkCmdPipelineBarrier2(vulkcomp.commandBuffers[vulkcomp.currentFrame], &dependencyInfo);

        VkRenderingAttachmentInfo colorAttachment{};
        colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        colorAttachment.imageView = vulkcomp.imageViews[imageIndex];
        colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.clearValue.color = { {0.0f, 0.0f, 0.0f, 1.0f} }; // Siyah ekran

        VkRenderingInfo renderingInfo{};
        renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
        renderingInfo.renderArea.extent = vulkcomp.swapchainvariables.extent;
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &colorAttachment;

        vkCmdBeginRendering(vulkcomp.commandBuffers[vulkcomp.currentFrame], &renderingInfo);

        vkCmdBindPipeline(vulkcomp.commandBuffers[vulkcomp.currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkcomp.pipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(vulkcomp.swapchainvariables.extent.width);
        viewport.height = static_cast<float>(vulkcomp.swapchainvariables.extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(vulkcomp.commandBuffers[vulkcomp.currentFrame], 0, 1, &viewport);

        // DYNAMIC SCISSOR AYARI
        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = vulkcomp.swapchainvariables.extent;
        vkCmdSetScissor(vulkcomp.commandBuffers[vulkcomp.currentFrame], 0, 1, &scissor);

        glm::mat4 Model = glm::mat4(1.0f);

       
        glm::mat4 View = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 5.0f),  
            glm::vec3(0.0f, 0.0f, 0.0f),  
            glm::vec3(0.0f, 1.0f, 0.0f)   
        );

        float aspect_ratio = 1700.0f / 900.0f;
        glm::mat4 Projection = glm::perspective(glm::radians(70.0f), aspect_ratio, 0.1f, 200.0f);
        Projection[1][1] *= -1.0f;
        glm::mat4 MVP = Projection * View * Model;


        GPUDrawPushConstants pushData{};
        pushData.worldMatrix = MVP; 
        pushData.vertexBuffer = meshbuffers.vertexBufferAddress;

        vkCmdPushConstants(vulkcomp.commandBuffers[vulkcomp.currentFrame], vulkcomp.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(GPUDrawPushConstants), &pushData);     

        vkCmdBindIndexBuffer(
            vulkcomp.commandBuffers[vulkcomp.currentFrame],
            meshbuffers.indexBuffer.buffer,
            0,
            VK_INDEX_TYPE_UINT32
        );

        vkCmdDrawIndexed(vulkcomp.commandBuffers[vulkcomp.currentFrame], static_cast<uint32_t>(recindices.size()), 1, 0, 0, 0); // 3 vertex = 1 üçgen
        vkCmdEndRendering(vulkcomp.commandBuffers[vulkcomp.currentFrame]);

        VkImageMemoryBarrier2 imageBarrier2{};
        imageBarrier2.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
        imageBarrier2.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        imageBarrier2.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
        imageBarrier2.dstStageMask = VK_PIPELINE_STAGE_2_NONE;
        imageBarrier2.dstAccessMask = 0;
        imageBarrier2.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        imageBarrier2.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        imageBarrier2.image = images[imageIndex];
        imageBarrier2.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageBarrier2.subresourceRange.levelCount = 1;
        imageBarrier2.subresourceRange.layerCount = 1;

        VkDependencyInfo dependencyInfo2{};
        dependencyInfo2.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
        dependencyInfo2.imageMemoryBarrierCount = 1;
        dependencyInfo2.pImageMemoryBarriers = &imageBarrier2;

        vkCmdPipelineBarrier2(vulkcomp.commandBuffers[vulkcomp.currentFrame], &dependencyInfo2);
        vkEndCommandBuffer(vulkcomp.commandBuffers[vulkcomp.currentFrame]);

        VkCommandBufferSubmitInfo commandBufferSubmitInfo{};
        commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
        commandBufferSubmitInfo.commandBuffer = vulkcomp.commandBuffers[vulkcomp.currentFrame];

        VkSemaphoreSubmitInfo waitSemaphoreInfo{};
        waitSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        waitSemaphoreInfo.semaphore = vulkcomp.imageAvailableSemaphores[vulkcomp.currentFrame];
        waitSemaphoreInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSemaphoreSubmitInfo signalSemaphoreInfo{};
        signalSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        signalSemaphoreInfo.semaphore = vulkcomp.renderFinishedSemaphores[imageIndex];
        signalSemaphoreInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo2 submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
        submitInfo.commandBufferInfoCount = 1;
        submitInfo.pCommandBufferInfos = &commandBufferSubmitInfo;
        submitInfo.waitSemaphoreInfoCount = 1;
        submitInfo.pWaitSemaphoreInfos = &waitSemaphoreInfo;
        submitInfo.signalSemaphoreInfoCount = 1;
        submitInfo.pSignalSemaphoreInfos = &signalSemaphoreInfo;

        vkQueueSubmit2(vulkcomp.graphicsQueue, 1, &submitInfo, vulkcomp.inFlightFences[vulkcomp.currentFrame]);

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &vulkcomp.renderFinishedSemaphores[imageIndex];

        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain;
        presentInfo.pImageIndices = &imageIndex; 

        vkQueuePresentKHR(vulkcomp.presentQueue, &presentInfo);

        vulkcomp.currentFrame = (vulkcomp.currentFrame + 1) % vulkcomp.MAX_FRAMES_IN_FLIGHT;
    }

}