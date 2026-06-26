#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/include/glfw3.h>

#include "Backend/vulkInstance.h"
#include "Backend/vulkDevice.h"
#include "Backend/vulkSwapchain.h"
#include "Backend/vulkWindow.h"
#include "Backend/vulkGraphicsPipeline.h"
#include "Backend/vulkShaderModule.h"
#include "Backend/vulkRendering.h"

namespace vulkBackend {
	void InitVulk();
	void DestroyVulk();

	void RenderSingleFrame();
	void WaitIdle();

	GLFWwindow* getwindow();
}