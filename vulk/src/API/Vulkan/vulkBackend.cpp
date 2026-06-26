#include "vulkBackend.h"

namespace vulkBackend {
	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugmessenger;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicaldevice;
	VkDevice logicaldevice;
	vulkSwapchain::SwapchainVariables swapchainvariables;
	std::vector<VkImageView> swapchainimageviews;
	VkPipeline pipeline;
	VkPipelineLayout pipelinelayout;
	vulkDevice::QueueFamilyIndices indices;
	VkCommandPool commandpool;
	std::vector<VkCommandBuffer> commandbuffers;
	std::vector<VkSemaphore> imageavailablesemaphores;
	std::vector<VkSemaphore> renderfinishedsemaphores;
	std::vector<VkFence> inflightfences;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkShaderModule vertshadermodule;
	VkShaderModule fragshadermodule;

	const int MAX_FRAMES_IN_FLIGHT = 2;
	size_t currentFrame = 0;

	const int WIDTH = 1280;
	const int HEIGHT = 720;

	void InitVulk() {
		glfwInit();
		instance = vulkInstance::create();
		debugmessenger = vulkInstance::createDebugMessenger(instance);

		window = vulkanWindow::createWindow(WIDTH, HEIGHT);
		surface = vulkanWindow::createSurface(instance, window);

		physicaldevice = vulkDevice::pickPhysicalDevice(instance, surface);
		indices = vulkDevice::findQueueFamilies(physicaldevice, surface);
		logicaldevice = vulkDevice::createLogicalDevice(surface, physicaldevice, indices);

		swapchainvariables = vulkSwapchain::create(instance, physicaldevice, logicaldevice, window, surface, indices);
		swapchainimageviews = vulkSwapchain::createImageViews(logicaldevice, swapchainvariables.images, swapchainvariables.format);

		pipelinelayout = vulkGraphicsPipeline::CreatePipelineLayout(logicaldevice);
		auto vertShaderCode = vulkShaderModule::readFile("res/Shaders/vert.spv");
		auto fragShaderCode = vulkShaderModule::readFile("res/Shaders/frag.spv");
		vertshadermodule = vulkShaderModule::createShaderModule(vertShaderCode, logicaldevice);
		fragshadermodule = vulkShaderModule::createShaderModule(fragShaderCode, logicaldevice);
		pipeline = vulkGraphicsPipeline::CreateGraphicsPipeline(logicaldevice, pipelinelayout, vertshadermodule, fragshadermodule, swapchainvariables.format);

		commandpool = vulkRendering::createCommandPool(logicaldevice, indices.graphicsFamily.value());

		graphicsQueue = vulkDevice::createGraphicsQueue(logicaldevice, indices);
		presentQueue = vulkDevice::createPresentQueue(logicaldevice, indices);

		commandbuffers.resize(MAX_FRAMES_IN_FLIGHT);
		imageavailablesemaphores.resize(swapchainvariables.images.size());
		renderfinishedsemaphores.resize(swapchainvariables.images.size());
		inflightfences.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			inflightfences[i] = vulkRendering::createFence(logicaldevice);
			commandbuffers[i] = vulkRendering::allocateCommandBuffer(logicaldevice, commandpool);
		}

		for (size_t i = 0; i < swapchainvariables.images.size(); i++) {
			imageavailablesemaphores[i] = vulkRendering::createSemaphore(logicaldevice);
			renderfinishedsemaphores[i] = vulkRendering::createSemaphore(logicaldevice);
		}
	}
	

	void DestroyVulk() {
		for (size_t i = 0; i < swapchainvariables.images.size(); i++) {
			vkDestroySemaphore(logicaldevice, renderfinishedsemaphores[i], nullptr);
			vkDestroySemaphore(logicaldevice, imageavailablesemaphores[i], nullptr);
		}
		
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyFence(logicaldevice, inflightfences[i], nullptr);
		}
		vkDestroyCommandPool(logicaldevice, commandpool, nullptr);

		vkDestroyPipeline(logicaldevice, pipeline, nullptr);
		vkDestroyShaderModule(logicaldevice, fragshadermodule, nullptr);
		vkDestroyShaderModule(logicaldevice, vertshadermodule, nullptr);
		vkDestroyPipelineLayout(logicaldevice, pipelinelayout, nullptr);

		for (auto imageView : swapchainimageviews) {
			vkDestroyImageView(logicaldevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(logicaldevice, swapchainvariables.swapchain, nullptr);

		vkDestroyDevice(logicaldevice, nullptr);

		if (debugmessenger != VK_NULL_HANDLE) {
			vulkInstance::clean(instance, debugmessenger);
		}

		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void RenderSingleFrame() {
		vulkRendering::DrawFrame(
			logicaldevice,
			swapchainvariables.swapchain,
			swapchainvariables.images,
			swapchainimageviews,
			swapchainvariables.format,       
			swapchainvariables.extent,
			pipeline,
			pipelinelayout,
			commandbuffers,
			graphicsQueue,
			presentQueue,
			currentFrame,                    
			MAX_FRAMES_IN_FLIGHT,            
			imageavailablesemaphores,       
			renderfinishedsemaphores,        
			inflightfences                   
		);

	}

	void WaitIdle() {
		vkDeviceWaitIdle(logicaldevice);
	}

	
	GLFWwindow* getwindow() { return window; }
}
