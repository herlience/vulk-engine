#include "vulkBackend.h"

namespace vulkBackend {

	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugmessenger;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicaldevice;
	frameComponents framecomp;

	vulkSwapchain::SwapchainVariables swapchainvariables;
	vulkDevice::QueueFamilyIndices indices;
	VkCommandPool commandpool;
	VkDeviceMemory vertexbuffermemory;

	VkShaderModule vertshadermodule;
	VkShaderModule fragshadermodule;

	const int WIDTH = 800;
	const int HEIGHT = 600;

	void InitVulk() {
		glfwInit();

		framecomp.MAX_FRAMES_IN_FLIGHT = 2;
		framecomp.currentFrame = 0;
		framecomp.vertices = vertices;

		instance = vulkInstance::create();
		debugmessenger = vulkInstance::createDebugMessenger(instance);

		window = vulkanWindow::createWindow(WIDTH, HEIGHT);
		surface = vulkanWindow::createSurface(instance, window);

		physicaldevice = vulkDevice::pickPhysicalDevice(instance, surface);
		indices = vulkDevice::findQueueFamilies(physicaldevice, surface);
		framecomp.device = vulkDevice::createLogicalDevice(surface, physicaldevice, indices, framecomp.fnCmdSetVertexInputEXT);

		swapchainvariables = vulkSwapchain::create(instance, physicaldevice, framecomp.device, window, surface, indices);
		framecomp.extent = swapchainvariables.extent;
		framecomp.imageViews = vulkSwapchain::createImageViews(framecomp.device, swapchainvariables.images, swapchainvariables.format);

		framecomp.layout = vulkGraphicsPipeline::CreatePipelineLayout(framecomp.device);
		auto vertShaderCode = vulkShaderModule::readFile("res/Shaders/vert.spv");
		auto fragShaderCode = vulkShaderModule::readFile("res/Shaders/frag.spv");
		vertshadermodule = vulkShaderModule::createShaderModule(vertShaderCode, framecomp.device);
		fragshadermodule = vulkShaderModule::createShaderModule(fragShaderCode, framecomp.device);
		framecomp.pipeline = vulkGraphicsPipeline::CreateGraphicsPipeline(framecomp.device, framecomp.layout, vertshadermodule, fragshadermodule, swapchainvariables.format);

		commandpool = vulkRendering::createCommandPool(framecomp.device, indices.graphicsFamily.value());

		framecomp.graphicsQueue = vulkDevice::createGraphicsQueue(framecomp.device, indices);
		framecomp.presentQueue = vulkDevice::createPresentQueue(framecomp.device, indices);

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeof(vertices[0]) * vertices.size();
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		framecomp.vertexbuffer = vulkBuffer::createVertexBuffer(framecomp.device, vertexbuffermemory, bufferInfo, physicaldevice);
		vulkBuffer::bindMemory(framecomp.device, framecomp.vertexbuffer, vertexbuffermemory);
		vulkBuffer::MapMemory(framecomp.device, vertexbuffermemory, vertices, bufferInfo);

		framecomp.commandBuffers.resize(framecomp.MAX_FRAMES_IN_FLIGHT);
		framecomp.imageAvailableSemaphores.resize(swapchainvariables.images.size());
		framecomp.renderFinishedSemaphores.resize(swapchainvariables.images.size());
		framecomp.inFlightFences.resize(framecomp.MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < framecomp.MAX_FRAMES_IN_FLIGHT; i++) {
			framecomp.inFlightFences[i] = vulkRendering::createFence(framecomp.device);
			framecomp.commandBuffers[i] = vulkRendering::allocateCommandBuffer(framecomp.device, commandpool);
		}

		for (size_t i = 0; i < swapchainvariables.images.size(); i++) {
			framecomp.imageAvailableSemaphores[i] = vulkRendering::createSemaphore(framecomp.device);
			framecomp.renderFinishedSemaphores[i] = vulkRendering::createSemaphore(framecomp.device);
		}
	}
	

	void DestroyVulk() {
		for (size_t i = 0; i < swapchainvariables.images.size(); i++) {
			vkDestroySemaphore(framecomp.device, framecomp.renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(framecomp.device, framecomp.imageAvailableSemaphores[i], nullptr);
		}
		
		for (size_t i = 0; i < framecomp.MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyFence(framecomp.device, framecomp.inFlightFences[i], nullptr);
		}
		vkDestroyCommandPool(framecomp.device, commandpool, nullptr);

		vkDestroyBuffer(framecomp.device, framecomp.vertexbuffer, nullptr);
		vkFreeMemory(framecomp.device, vertexbuffermemory, nullptr);

		vkDestroyPipeline(framecomp.device, framecomp.pipeline, nullptr);
		vkDestroyShaderModule(framecomp.device, fragshadermodule, nullptr);
		vkDestroyShaderModule(framecomp.device, vertshadermodule, nullptr);
		vkDestroyPipelineLayout(framecomp.device, framecomp.layout, nullptr);

		for (auto imageView : framecomp.imageViews) {
			vkDestroyImageView(framecomp.device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(framecomp.device, swapchainvariables.swapchain, nullptr);

		vkDestroyDevice(framecomp.device, nullptr);

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
			framecomp,
			swapchainvariables.swapchain,
			swapchainvariables.images,
			swapchainvariables.format,       
			swapchainvariables.extent
		);

	}

	void WaitIdle() {
		vkDeviceWaitIdle(framecomp.device);
	}

	
	GLFWwindow* getwindow() { return window; }
}
