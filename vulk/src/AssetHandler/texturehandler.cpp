#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>
#include "texturehandler.h"
#include "../Vulkan/Backend/vulkBuffer.h"

namespace texturehandler {
	texture loadtexturefile(const char* filepath, VulkComponents vulkcomp) {
		texture tex;
		int w, h, channels;
		
		stbi_uc* pixels = stbi_load(filepath, &w, &h, &channels, STBI_rgb_alpha);

		if (!pixels) {
			std::cerr << "resim yuklenemedi: " << filepath << std::endl;
			return tex;
		}

		tex.width = static_cast<uint32_t>(w);
		tex.height = static_cast<uint32_t>(h);
		VkDeviceSize imagesize = tex.width * tex.height * 4;
		
		AllocatedBuffer stagingbuffer = vulkBuffer::create_buffer(imagesize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_AUTO_PREFER_HOST, vulkcomp.allocator);
		memcpy(stagingbuffer.info.pMappedData, pixels, static_cast<size_t>(imagesize));

		stbi_image_free(pixels);

		tex.texextent.depth = 1;
		tex.texextent.width = tex.width;
		tex.texextent.height = tex.height;

		tex.image = vulkBuffer::create_image(tex.texextent, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, vulkcomp.allocator);
		
		VkCommandBuffer cmd = vulkBuffer::begin_single_time_commands(vulkcomp);

		vulkBuffer::transition_image_layout(cmd, tex.image.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		vulkBuffer::copy_buffer_to_image(cmd, stagingbuffer.buffer, tex.image.image, tex.width, tex.height);
		vulkBuffer::transition_image_layout(cmd, tex.image.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vulkBuffer::end_single_time_commands(vulkcomp, cmd);
		vulkBuffer::destroy_buffer(stagingbuffer, vulkcomp.allocator);

		return tex;
	}
}