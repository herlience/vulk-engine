#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_vulkan.h>
#include <imgui/imgui_impl_glfw.h>
#include "../Vulkan/vulkTypes.h"
#include <iostream>

namespace Imgui {
	void init(
		VkInstance instance, 
		VkPhysicalDevice physicaldevice, 
		VkDevice device, 
		QueueFamilyIndices familyindex, 
		VkQueue graphicsqueue, 
		VkDescriptorPool imguipool, 
		SwapchainVariables swapchainvar,
		VulkComponents vulkcomp,
		GLFWwindow* window
	);
	void destroy();
}