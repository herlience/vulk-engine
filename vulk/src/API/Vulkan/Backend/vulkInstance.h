#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace vulkInstance {
	inline VkInstance vulkInstance = VK_NULL_HANDLE;
	inline VkDebugUtilsMessengerEXT vulkDebugMessenger = VK_NULL_HANDLE;

	void create();
	void clean();

	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();

	VkInstance getInstance();

	// validation settings
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
}

