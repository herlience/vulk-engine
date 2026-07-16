#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_vulkan.h>
#include <imgui/imgui_impl_glfw.h>
#include "../Vulkan/vulkTypes.h"
#include <iostream>

namespace Imgui {
	void init(
		VulkComponents vulkcomp,
		QueueFamilyIndices familyindex,
		GLFWwindow* window
	);
	void destroy();
}