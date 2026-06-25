#define GLFW_INCLUDE_VULKAN
#pragma once

#include <GLFW/include/glfw3.h>

namespace vulkanWindow {
	void Destroy(GLFWwindow* vulk_window);
	GLFWwindow* createWindow(int WIDTH, int HEIGHT);
	VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow* vulk_window);
}

