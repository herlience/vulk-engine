

#pragma once

#include <imgui.h>

#include <imconfig.h>
#include <imgui_internal.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
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