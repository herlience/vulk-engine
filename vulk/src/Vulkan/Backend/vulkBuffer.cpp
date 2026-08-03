#include "vulkBuffer.h"

namespace vulkBuffer {
    AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, VmaAllocator _allocator) {
        VkBufferCreateInfo bufferInfo = { .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufferInfo.pNext = nullptr;
        bufferInfo.size = allocSize;

        bufferInfo.usage = usage;

        VmaAllocationCreateInfo vmaallocInfo = {};
        vmaallocInfo.usage = memoryUsage;
        vmaallocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
        AllocatedBuffer newBuffer;

        VK_CHECK(vmaCreateBuffer(_allocator, &bufferInfo, &vmaallocInfo, &newBuffer.buffer, &newBuffer.allocation, &newBuffer.info));

        return newBuffer;
    }

    AllocatedImage create_image(VkExtent3D size, VkFormat format, VkImageUsageFlags usage, VmaAllocator allocator) {
        AllocatedImage newImage;
        newImage.imageFormat = format;
        newImage.imageExtent = size;

        VkImageCreateInfo imgInfo = { .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
        imgInfo.pNext = nullptr;
        imgInfo.imageType = VK_IMAGE_TYPE_2D;
        imgInfo.format = format;
        imgInfo.extent = size;
        imgInfo.mipLevels = 1;
        imgInfo.arrayLayers = 1;
        imgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL; 
        imgInfo.usage = usage;

        VmaAllocationCreateInfo allocInfo = {};
        
        allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        allocInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VK_CHECK(vmaCreateImage(allocator, &imgInfo, &allocInfo, &newImage.image, &newImage.allocation, nullptr));

        return newImage;
    }

    VkImageView createimageview(AllocatedImage image, VkDevice device, VkImageAspectFlags aspectmask) {
        VkImageViewCreateInfo viewInfo = { .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
        viewInfo.image = image.image; 
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = image.imageFormat; 

        
        viewInfo.subresourceRange.aspectMask = aspectmask;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;

        VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &imageView));

        return imageView;
    }

    VkSampler createtexturesampler(VkDevice device) {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        
        samplerInfo.anisotropyEnable = VK_FALSE; 
        samplerInfo.maxAnisotropy = 1.0f;

        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE; 
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

        
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        VkSampler textureSampler;
        if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
            std::cerr << "We have a problem on Texture Sampler!" << std::endl;
        }

        return textureSampler;
    }

    VkCommandBuffer begin_single_time_commands(VulkComponents vulkcomp) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vulkcomp.commandpool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(vulkcomp.device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }
    void end_single_time_commands(VulkComponents vulkcomp, VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(vulkcomp.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        
        vkQueueWaitIdle(vulkcomp.graphicsQueue);

        vkFreeCommandBuffers(vulkcomp.device, vulkcomp.commandpool, 1, &commandBuffer);
    }

    void copy_buffer_to_image(VkCommandBuffer cmd, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
    {
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { width, height, 1 };

        vkCmdCopyBufferToImage(
            cmd,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );
    }

    void transition_image_layout(VkCommandBuffer cmd, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }

        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            std::cerr << "Desteklenmeyen layout gecisi!" << std::endl;
            return;
        }

        vkCmdPipelineBarrier(
            cmd,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );
    }

    void destroy_buffer(const AllocatedBuffer& buffer, VmaAllocator _allocator) {
        vmaDestroyBuffer(_allocator, buffer.buffer, buffer.allocation);
    }

    void destroy_image(const AllocatedImage& image, VmaAllocator allocator) {
        vmaDestroyImage(allocator, image.image, image.allocation);
    }
    
    void destroy_imageview(VkDevice device, VkImageView imageview) {
        vkDestroyImageView(device, imageview, nullptr);
    }
}