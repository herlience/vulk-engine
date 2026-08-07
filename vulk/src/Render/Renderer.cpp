#include "Renderer.h"
#include <algorithm>

namespace Renderer {
    void DrawFrame(
        VulkComponents& vulkcomp
    ) {
        vkWaitForFences(vulkcomp.device, 1, &vulkcomp.inFlightFences[vulkcomp.currentFrame], VK_TRUE, UINT64_MAX);
        vkResetFences(vulkcomp.device, 1, &vulkcomp.inFlightFences[vulkcomp.currentFrame]);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(
            vulkcomp.device,
            vulkcomp.swapchainvariables.swapchain,
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
        imageBarrier.image = vulkcomp.swapchainvariables.images[imageIndex];
        imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageBarrier.subresourceRange.levelCount = 1;
        imageBarrier.subresourceRange.layerCount = 1;

        VkImageMemoryBarrier2 depthBarrier{};
        depthBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
        depthBarrier.srcStageMask = VK_PIPELINE_STAGE_2_NONE;
        depthBarrier.srcAccessMask = VK_ACCESS_2_NONE;
        depthBarrier.dstStageMask = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT;
        depthBarrier.dstAccessMask = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        depthBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthBarrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
        depthBarrier.image = vulkcomp.depthimage.image;
        depthBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        depthBarrier.subresourceRange.baseMipLevel = 0;
        depthBarrier.subresourceRange.levelCount = 1;
        depthBarrier.subresourceRange.baseArrayLayer = 0;
        depthBarrier.subresourceRange.layerCount = 1;

        VkImageMemoryBarrier2 barriers[2] = { imageBarrier, depthBarrier };

        VkDependencyInfo dependencyInfo{};
        dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
        dependencyInfo.imageMemoryBarrierCount = 2;
        dependencyInfo.pImageMemoryBarriers = barriers;

        vkCmdPipelineBarrier2(vulkcomp.commandBuffers[vulkcomp.currentFrame], &dependencyInfo);

        VkRenderingAttachmentInfo colorAttachment{};
        colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        colorAttachment.imageView = vulkcomp.imageViews[imageIndex];
        colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.clearValue.color = { {0.0f, 0.0f, 0.0f, 1.0f} }; // Siyah ekran

        VkRenderingAttachmentInfo depthAttachmentInfo{};
        depthAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        depthAttachmentInfo.imageView = vulkcomp.depthimageview;
        depthAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
        depthAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        depthAttachmentInfo.clearValue.depthStencil = { 1.0f, 0 };

        VkRenderingInfo renderingInfo{};
        renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
        renderingInfo.renderArea = { {0, 0}, vulkcomp.swapchainvariables.extent };
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &colorAttachment;
        renderingInfo.pDepthAttachment = &depthAttachmentInfo;

        vkCmdBeginRendering(vulkcomp.commandBuffers[vulkcomp.currentFrame], &renderingInfo);

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

        float width = static_cast<float>(vulkcomp.swapchainvariables.extent.width);
        float height = static_cast<float>(vulkcomp.swapchainvariables.extent.height);
        glm::mat4 viewproj = CameraSystem::getViewProjectionMatrix(width, height);

        AssetHandler::syncpendingobjects();

        vkCmdBindPipeline(vulkcomp.commandBuffers[vulkcomp.currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkcomp.mainpipeline);
        vkCmdBindDescriptorSets(vulkcomp.commandBuffers[vulkcomp.currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkcomp.mainlayout, 0, 1, &vulkcomp.descriptorset, 0, nullptr);

        for (size_t i = 0; i < Renderer::m_drawlist.size(); ++i) {
            const auto& obj = Renderer::m_drawlist[i];

            GPUDrawPushConstants pushData{};
            glm::mat4 modelMat = Renderer::m_gameobjectlist[i].getModelMatrix();
            pushData.worldMatrix = viewproj * modelMat;
            pushData.vertexBuffer = obj.vertexBufferAddress;
            pushData.textureIndex = obj.textureIndex;

            vkCmdPushConstants(vulkcomp.commandBuffers[vulkcomp.currentFrame], vulkcomp.mainlayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(GPUDrawPushConstants), &pushData);

            vkCmdBindIndexBuffer(
                vulkcomp.commandBuffers[vulkcomp.currentFrame],
                obj.indexBuffer,
                0,
                VK_INDEX_TYPE_UINT32
            );
            
            vkCmdDrawIndexed(
                vulkcomp.commandBuffers[vulkcomp.currentFrame],
                obj.indexCount,
                1,
                0, 
                0,
                0
            );
        }

        vkCmdBindPipeline(vulkcomp.commandBuffers[vulkcomp.currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, vulkcomp.gridpipeline);

        GridPushConstants pushData{};
        pushData.viewproj = viewproj;
        pushData.camerapos = CameraSystem::mainCamera.position;


        vkCmdPushConstants(
            vulkcomp.commandBuffers[vulkcomp.currentFrame],
            vulkcomp.gridlayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0, sizeof(GridPushConstants),
            &pushData
        );

        vkCmdDraw(vulkcomp.commandBuffers[vulkcomp.currentFrame], 6, 1, 0, 0);

        AssetHandler::RenderImGui(vulkcomp, vulkcomp.commandBuffers[vulkcomp.currentFrame]);

        vkCmdEndRendering(vulkcomp.commandBuffers[vulkcomp.currentFrame]);

        VkImageMemoryBarrier2 imageBarrier2{};
        imageBarrier2.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
        imageBarrier2.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        imageBarrier2.srcAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
        imageBarrier2.dstStageMask = VK_PIPELINE_STAGE_2_NONE;
        imageBarrier2.dstAccessMask = 0;
        imageBarrier2.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        imageBarrier2.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        imageBarrier2.image = vulkcomp.swapchainvariables.images[imageIndex];
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
        waitSemaphoreInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT;

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
        presentInfo.pSwapchains = &vulkcomp.swapchainvariables.swapchain;
        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(vulkcomp.presentQueue, &presentInfo);

        vulkcomp.currentFrame = (vulkcomp.currentFrame + 1) % vulkcomp.MAX_FRAMES_IN_FLIGHT;
    }

    void removeRenderObjectsByIndexBuffer(VkBuffer buffer) {
        m_drawlist.erase(
            std::remove_if(m_drawlist.begin(), m_drawlist.end(),
                [buffer](const RenderObject& o) { return o.indexBuffer == buffer; }),
            m_drawlist.end()
        );
    }
}