#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace vulkInstance {

	VkInstance create();
	VkDebugUtilsMessengerEXT createDebugMessenger(VkInstance vulkInstance);
	void clean(VkInstance vulkInstance, VkDebugUtilsMessengerEXT vulkDebugMessenger);

	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();


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

