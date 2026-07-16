#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/include/glfw3.h>
#include <VMA/vk_mem_alloc.h>
#include "vulkTypes.h"
#include <imgui/imgui.h>
#include "Backend/vulkInstance.h"
#include "Backend/vulkDevice.h"
#include "Backend/vulkSwapchain.h"
#include "Backend/vulkWindow.h"
#include "Backend/vulkGraphicsPipeline.h"
#include "Backend/vulkShaderModule.h"
#include "Backend/vulkRendering.h"
#include "Backend/vulkBuffer.h"
#include "GeoData.h"

namespace vulkBackend {
	void InitVulk();
	void DestroyVulk();
	void WaitIdle();

	GLFWwindow* getwindow();
	VulkComponents getvulkcomp();
	QueueFamilyIndices getfamilyindex();
}