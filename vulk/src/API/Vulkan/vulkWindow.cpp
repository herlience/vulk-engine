#define GLFW_INCLUDE_VULKAN
#include <GLFW/include/glfw3.h>

#include <iostream>

#include "vulkWindow.h"
#include "Backend/vulkanBootstrap.h"

namespace vulkanWindow {
	bool Init() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		vulk_window = glfwCreateWindow(WIDTH, HEIGHT, "vulk", nullptr, nullptr);

		if (!vulk_window) { return false; }

		if (glfwCreateWindowSurface(vulkanBootstrap::getInstance(), vulk_window, nullptr, &vulkSurface) != VK_SUCCESS) {
			std::cerr << "We have a problem on Surface!" << std::endl;
			return false;
		}

		return true;
	}

	void Destroy() {
		glfwDestroyWindow(vulk_window);
		glfwTerminate();
	}

	GLFWwindow* getWindow() {
		return vulk_window;
	}

	VkSurfaceKHR getSurface() {
		return vulkSurface;
	}
}
