#include "vulkBackend.h"

namespace vulkBackend {

	GLFWwindow* window;

	SwapchainVariables swapchainvariables;
	QueueFamilyIndices indices;
	inline VulkComponents vulkcomp;

	VkShaderModule mainvertshadermodule;
	VkShaderModule mainfragshadermodule;

	VkShaderModule gridvertshadermodule;
	VkShaderModule gridfragshadermodule;

	const int WIDTH = 1200;
	const int HEIGHT = 800;

	void InitVulk() {
		glfwInit();

		vulkcomp.MAX_FRAMES_IN_FLIGHT = 2;
		vulkcomp.currentFrame = 0;

		vulkcomp.instance = vulkInstance::create();
		vulkcomp.debugmessenger = vulkInstance::createDebugMessenger(vulkcomp.instance);

		window = vulkanWindow::createWindow(WIDTH, HEIGHT);
		vulkcomp.surface = vulkanWindow::createSurface(vulkcomp.instance, window);

		vulkcomp.physicaldevice = vulkDevice::pickPhysicalDevice(vulkcomp.instance, vulkcomp.surface);
		indices = vulkDevice::findQueueFamilies(vulkcomp.physicaldevice, vulkcomp.surface);
		vulkcomp.device = vulkDevice::createLogicalDevice(vulkcomp.surface, vulkcomp.physicaldevice, indices, vulkcomp.fnCmdSetVertexInputEXT);

		swapchainvariables = vulkSwapchain::create(vulkcomp.instance, vulkcomp.physicaldevice, vulkcomp.device, window, vulkcomp.surface, indices);
		vulkcomp.swapchainvariables = swapchainvariables;
		vulkcomp.imageViews = vulkSwapchain::createImageViews(vulkcomp.device, swapchainvariables.images, swapchainvariables.format);

		vulkcomp.maindescriptorsetlayout = vulkDescriptors::create_descriptor_set_layout(vulkcomp.device, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

		vulkcomp.descriptorpool = vulkDescriptors::create_descriptor_pool(vulkcomp.device);
		vulkcomp.descriptorset = vulkDescriptors::create_descriptor_set(vulkcomp.device, vulkcomp.maindescriptorsetlayout, vulkcomp.descriptorpool);

		// Graphics Pipelines
		vulkcomp.mainlayout = vulkGraphicsPipeline::CreatePipelineLayout(vulkcomp, VK_TRUE, static_cast<uint32_t>(sizeof(GPUDrawPushConstants)));
		auto mainvertShaderCode = vulkShaderModule::readFile("res/Shaders/vert.spv");
		auto mainfragShaderCode = vulkShaderModule::readFile("res/Shaders/frag.spv");
		mainvertshadermodule = vulkShaderModule::createShaderModule(mainvertShaderCode, vulkcomp.device);
		mainfragshadermodule = vulkShaderModule::createShaderModule(mainfragShaderCode, vulkcomp.device);
		vulkcomp.mainpipeline = vulkGraphicsPipeline::CreateGraphicsPipeline(
			vulkcomp.device, 
			vulkcomp.mainlayout, 
			mainvertshadermodule, 
			mainfragshadermodule, 
			swapchainvariables.format,
			VK_TRUE,
			VK_FALSE,
			VK_BLEND_FACTOR_ONE,
			VK_BLEND_FACTOR_ZERO
		);

		vulkcomp.gridlayout = vulkGraphicsPipeline::CreatePipelineLayout(vulkcomp, VK_FALSE, static_cast<uint32_t>(sizeof(GridPushConstants)));
		auto gridvertShaderCode = vulkShaderModule::readFile("res/Shaders/grid.vert.spv");
		auto gridfragShaderCode = vulkShaderModule::readFile("res/Shaders/grid.frag.spv");
		gridvertshadermodule = vulkShaderModule::createShaderModule(gridvertShaderCode, vulkcomp.device);
		gridfragshadermodule = vulkShaderModule::createShaderModule(gridfragShaderCode, vulkcomp.device);
		vulkcomp.gridpipeline = vulkGraphicsPipeline::CreateGraphicsPipeline(
			vulkcomp.device, 
			vulkcomp.gridlayout, 
			gridvertshadermodule, 
			gridfragshadermodule, 
			swapchainvariables.format,
			VK_FALSE,
			VK_TRUE,
			VK_BLEND_FACTOR_SRC_ALPHA,
			VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
		);
		///

		vulkcomp.commandpool = vulkRendering::createCommandPool(vulkcomp.device, indices.graphicsFamily.value());

		vulkcomp.graphicsQueue = vulkDevice::createGraphicsQueue(vulkcomp.device, indices);
		vulkcomp.presentQueue = vulkDevice::createPresentQueue(vulkcomp.device, indices);

		VmaVulkanFunctions vulkanFunctions = {};
		vulkanFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
		vulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

		VmaAllocatorCreateInfo allocatorCreateInfo = {};
		allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
		allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		allocatorCreateInfo.physicalDevice = vulkcomp.physicaldevice;
		allocatorCreateInfo.device = vulkcomp.device;
		allocatorCreateInfo.instance = vulkcomp.instance;
		allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;
		allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

		if (vmaCreateAllocator(&allocatorCreateInfo, &vulkcomp.allocator) != VK_SUCCESS) {
			std::cout << "we got a problem about allocator" << std::endl;
		}

		vulkcomp.threedextenttanimlama();

		vulkcomp.defaulttexturesampler = vulkBuffer::createtexturesampler(vulkcomp.device);

		vulkcomp.depthimage = vulkBuffer::create_image(vulkcomp.threedextent, VK_FORMAT_D32_SFLOAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, vulkcomp.allocator);
		vulkcomp.depthimageview = vulkBuffer::createimageview(vulkcomp.depthimage, vulkcomp.device, VK_IMAGE_ASPECT_DEPTH_BIT);
		
		vulkcomp.commandBuffers.resize(vulkcomp.MAX_FRAMES_IN_FLIGHT);
		vulkcomp.imageAvailableSemaphores.resize(swapchainvariables.images.size());
		vulkcomp.renderFinishedSemaphores.resize(swapchainvariables.images.size());
		vulkcomp.inFlightFences.resize(vulkcomp.MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < vulkcomp.MAX_FRAMES_IN_FLIGHT; i++) {
			vulkcomp.inFlightFences[i] = vulkRendering::createFence(vulkcomp.device);
			vulkcomp.commandBuffers[i] = vulkRendering::allocateCommandBuffer(vulkcomp.device, vulkcomp.commandpool);
		}

		for (size_t i = 0; i < swapchainvariables.images.size(); i++) {
			vulkcomp.imageAvailableSemaphores[i] = vulkRendering::createSemaphore(vulkcomp.device);
			vulkcomp.renderFinishedSemaphores[i] = vulkRendering::createSemaphore(vulkcomp.device);
		}
	}
	

	void DestroyVulk(VulkComponents& vulkcomp) {
		texturehandler::destroy_textures(vulkcomp);
		AssetHandler::cleanup(vulkcomp.allocator);
		Renderer::clearDrawlist();

		for (size_t i = 0; i < swapchainvariables.images.size(); i++) {
			vkDestroySemaphore(vulkcomp.device, vulkcomp.renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(vulkcomp.device, vulkcomp.imageAvailableSemaphores[i], nullptr);
		}

		for (size_t i = 0; i < vulkcomp.MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyFence(vulkcomp.device, vulkcomp.inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(vulkcomp.device, vulkcomp.commandpool, nullptr);

		vulkBuffer::destory_sampler(vulkcomp.device, vulkcomp.defaulttexturesampler);

		vulkDescriptors::destroy_all_descriptor_components(vulkcomp.device, vulkcomp.descriptorpool, vulkcomp.maindescriptorsetlayout);

		vkDestroyBuffer(vulkcomp.device, vulkcomp.vertexbuffer, nullptr);
		vkFreeMemory(vulkcomp.device, vulkcomp.vertexbuffermemory, nullptr);

		vkDestroyPipeline(vulkcomp.device, vulkcomp.mainpipeline, nullptr);
		vkDestroyShaderModule(vulkcomp.device, mainfragshadermodule, nullptr);
		vkDestroyShaderModule(vulkcomp.device, mainvertshadermodule, nullptr);
		vkDestroyPipelineLayout(vulkcomp.device, vulkcomp.mainlayout, nullptr);

		vkDestroyPipeline(vulkcomp.device, vulkcomp.gridpipeline, nullptr);
		vkDestroyShaderModule(vulkcomp.device, gridfragshadermodule, nullptr);
		vkDestroyShaderModule(vulkcomp.device, gridvertshadermodule, nullptr);
		vkDestroyPipelineLayout(vulkcomp.device, vulkcomp.gridlayout, nullptr);

		vulkSwapchain::clean(vulkcomp);

		vmaDestroyAllocator(vulkcomp.allocator);

		vkDestroyDevice(vulkcomp.device, nullptr);

		if (vulkcomp.debugmessenger != VK_NULL_HANDLE) {
			vulkInstance::clean(vulkcomp.instance, vulkcomp.debugmessenger);
		}

		vkDestroySurfaceKHR(vulkcomp.instance, vulkcomp.surface, nullptr);
		vkDestroyInstance(vulkcomp.instance, nullptr);

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WaitIdle() {
		vkDeviceWaitIdle(vulkcomp.device);
	}

	
	GLFWwindow* getwindow() { return window; }
	VulkComponents getvulkcomp() { return vulkcomp; }
	QueueFamilyIndices getfamilyindex() { return indices; }
}
