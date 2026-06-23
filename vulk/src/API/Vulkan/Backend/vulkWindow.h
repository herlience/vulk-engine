#define GLFW_INCLUDE_VULKAN
#pragma once

#include <GLFW/include/glfw3.h>

namespace vulkanWindow {
	inline GLFWwindow* vulk_window = nullptr;
	inline VkSurfaceKHR vulkSurface = VK_NULL_HANDLE;

	inline const int WIDTH = 1280;
	inline const int HEIGHT = 720;
	
	bool Init();
	void Destroy();
	GLFWwindow* getWindow();
	VkSurfaceKHR getSurface();
}

