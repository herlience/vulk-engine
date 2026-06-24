#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>


namespace vulkShaderModule {
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void destroyShaderModule(VkShaderModule shaderModule, VkDevice device);

	std::vector<char> readFile(const std::string& filename);
}
