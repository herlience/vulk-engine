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

    VkImageView createdepthimageview(AllocatedImage depthimage, VkDevice device) {
        VkImageViewCreateInfo viewInfo = { .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
        viewInfo.image = depthimage.image; 
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = depthimage.imageFormat; 

        
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView _depthImageView;

        VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &_depthImageView));

        return _depthImageView;
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