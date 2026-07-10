#include "vulkBackend.h"

namespace vulkBackend {

	GLFWwindow* window;

	SwapchainVariables swapchainvariables;
	vulkDevice::QueueFamilyIndices indices;
	VulkComponents vulkcomp;

	VkShaderModule vertshadermodule;
	VkShaderModule fragshadermodule;

	const int WIDTH = 800;
	const int HEIGHT = 600;

	void InitVulk() {
		glfwInit();

		vulkcomp.MAX_FRAMES_IN_FLIGHT = 2;
		vulkcomp.currentFrame = 0;
		vulkcomp.vertices = vertices;

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

		vulkcomp.layout = vulkGraphicsPipeline::CreatePipelineLayout(vulkcomp.device);
		auto vertShaderCode = vulkShaderModule::readFile("res/Shaders/vert.spv");
		auto fragShaderCode = vulkShaderModule::readFile("res/Shaders/frag.spv");
		vertshadermodule = vulkShaderModule::createShaderModule(vertShaderCode, vulkcomp.device);
		fragshadermodule = vulkShaderModule::createShaderModule(fragShaderCode, vulkcomp.device);
		vulkcomp.pipeline = vulkGraphicsPipeline::CreateGraphicsPipeline(vulkcomp.device, vulkcomp.layout, vertshadermodule, fragshadermodule, swapchainvariables.format);

		vulkcomp.commandpool = vulkRendering::createCommandPool(vulkcomp.device, indices.graphicsFamily.value());

		vulkcomp.graphicsQueue = vulkDevice::createGraphicsQueue(vulkcomp.device, indices);
		vulkcomp.presentQueue = vulkDevice::createPresentQueue(vulkcomp.device, indices);

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeof(vertices[0]) * vertices.size();
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		vulkcomp.vertexbuffer = vulkBuffer::createVertexBuffer(vulkcomp.device, vulkcomp.vertexbuffermemory, bufferInfo, vulkcomp.physicaldevice);
		vulkBuffer::bindMemory(vulkcomp.device, vulkcomp.vertexbuffer, vulkcomp.vertexbuffermemory);
		vulkBuffer::MapMemory(vulkcomp.device, vulkcomp.vertexbuffermemory, vertices, bufferInfo);

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
	

	void DestroyVulk() {
		for (size_t i = 0; i < swapchainvariables.images.size(); i++) {
			vkDestroySemaphore(vulkcomp.device, vulkcomp.renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(vulkcomp.device, vulkcomp.imageAvailableSemaphores[i], nullptr);
		}
		
		for (size_t i = 0; i < vulkcomp.MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyFence(vulkcomp.device, vulkcomp.inFlightFences[i], nullptr);
		}
		vkDestroyCommandPool(vulkcomp.device, vulkcomp.commandpool, nullptr);

		vkDestroyBuffer(vulkcomp.device, vulkcomp.vertexbuffer, nullptr);
		vkFreeMemory(vulkcomp.device, vulkcomp.vertexbuffermemory, nullptr);

		vkDestroyPipeline(vulkcomp.device, vulkcomp.pipeline, nullptr);
		vkDestroyShaderModule(vulkcomp.device, fragshadermodule, nullptr);
		vkDestroyShaderModule(vulkcomp.device, vertshadermodule, nullptr);
		vkDestroyPipelineLayout(vulkcomp.device, vulkcomp.layout, nullptr);

		for (auto imageView : vulkcomp.imageViews) {
			vkDestroyImageView(vulkcomp.device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(vulkcomp.device, swapchainvariables.swapchain, nullptr);

		vkDestroyDevice(vulkcomp.device, nullptr);

		if (vulkcomp.debugmessenger != VK_NULL_HANDLE) {
			vulkInstance::clean(vulkcomp.instance, vulkcomp.debugmessenger);
		}

		vkDestroySurfaceKHR(vulkcomp.instance, vulkcomp.surface, nullptr);
		vkDestroyInstance(vulkcomp.instance, nullptr);

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void RenderSingleFrame() {
		vulkRendering::DrawFrame(
			vulkcomp,
			swapchainvariables.swapchain,
			swapchainvariables.images,
			swapchainvariables.format,       
			swapchainvariables.extent
		);

	}

	void WaitIdle() {
		vkDeviceWaitIdle(vulkcomp.device);
	}

	
	GLFWwindow* getwindow() { return window; }
}
