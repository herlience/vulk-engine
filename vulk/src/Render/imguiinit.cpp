#define IMGUI_IMPL_VULKAN_HAS_DYNAMIC_RENDERING

#include "imguiinit.h"

namespace Imgui {
	static void check_result(VkResult err) {
		if (err == VK_SUCCESS) { return; }

		std::cerr << "ImGui: " << err << std::endl;
		if (err < 0) { abort(); }
	}

	void init(
		VkInstance instance,
		VkPhysicalDevice physicaldevice,
		VkDevice device,
		QueueFamilyIndices familyindex,
		VkQueue graphicsqueue,
		VkDescriptorPool imguipool,
		SwapchainVariables swapchainvar,
		VulkComponents vulkcomp,
		GLFWwindow* window
	) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForVulkan(window, true);

		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = instance;
		init_info.PhysicalDevice = physicaldevice;
		init_info.Device = device;
		init_info.QueueFamily = familyindex.graphicsFamily.value();
		init_info.Queue = graphicsqueue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = VK_NULL_HANDLE;
		init_info.MinImageCount = swapchainvar.imagecount;
		init_info.ImageCount = swapchainvar.imagecount;
		init_info.UseDynamicRendering = true;
		
		VkPipelineRenderingCreateInfoKHR renderingCreateInfo{};
		renderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		renderingCreateInfo.colorAttachmentCount = 1;
		renderingCreateInfo.pColorAttachmentFormats = &swapchainvar.format;

		ImGui_ImplVulkan_PipelineInfo pipelineinfomain = {};
		pipelineinfomain.PipelineRenderingCreateInfo = renderingCreateInfo;
		pipelineinfomain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		VkDynamicState dynamicStates[] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		ImVector<VkDynamicState> imstate;

		for (auto state : dynamicStates) {
			imstate.push_back(state);
		}

		pipelineinfomain.ExtraDynamicStates = imstate;
		pipelineinfomain.RenderPass = VK_NULL_HANDLE;
		
		init_info.PipelineInfoMain = pipelineinfomain;
		init_info.ApiVersion = VK_API_VERSION_1_4;
		init_info.CheckVkResultFn = check_result;
		init_info.DescriptorPoolSize = 1000;
		init_info.Allocator = nullptr;
		init_info.MinAllocationSize = 0;

		ImGui_ImplVulkan_Init(&init_info);
	}

	void destroy() {
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}