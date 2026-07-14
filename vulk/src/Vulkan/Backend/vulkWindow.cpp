#define GLFW_INCLUDE_VULKAN
#include <GLFW/include/glfw3.h>

#include <iostream>

#include "vulkWindow.h"
#include "vulkInstance.h"

namespace vulkanWindow {

	void Destroy(GLFWwindow* vulk_window) {
		glfwDestroyWindow(vulk_window);
		glfwTerminate();
	}

	GLFWwindow* createWindow( int WIDTH, int HEIGHT) {
		GLFWwindow* vulk_window = nullptr;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		vulk_window = glfwCreateWindow(WIDTH, HEIGHT, "vulk", nullptr, nullptr);

		return vulk_window;
	}

	VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow* vulk_window) {
		VkSurfaceKHR vulkSurface = VK_NULL_HANDLE;
		if (glfwCreateWindowSurface(instance, vulk_window, nullptr, &vulkSurface) != VK_SUCCESS) {
			std::cerr << "We have a problem on Surface!" << std::endl;
			return VK_NULL_HANDLE;
		}
		return vulkSurface;
	}

	bool ShouldClose(GLFWwindow* vulk_window) {
		return glfwWindowShouldClose(vulk_window);
	}

	void PollEvents() {
		glfwPollEvents();
	}
}
