#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "Backend.h"
#include "../Vulkan/vulkBackend.h"
#include "../Render/ObjectRenderer.h"
#include "../Render/Renderer.h"
#include "../Camera/Camera.h"
#include "../Objects/GameObject.h"
#include "../AssetHandler/AssetHandler.h"
#include "../Render/imguiinit.h"

namespace Backend {
	void RunEngine() {
		vulkBackend::InitVulk();
		VulkComponents vulkcomp = vulkBackend::getvulkcomp();
		GLFWwindow* window = vulkBackend::getwindow();
		QueueFamilyIndices familyindex = vulkBackend::getfamilyindex();
		Imgui::init(vulkcomp, familyindex, window);

		CameraSystem::updateProjectionMatrix(
			static_cast<float>(vulkcomp.swapchainvariables.extent.width),
			static_cast<float>(vulkcomp.swapchainvariables.extent.height)
		);
		CameraSystem::updateViewMatrix();

		while (!vulkanWindow::ShouldClose(window)) {
			vulkanWindow::PollEvents();
			Renderer::DrawFrame(vulkcomp);
		}

		vulkBackend::WaitIdle();
		Imgui::destroy();
		vulkBackend::DestroyVulk();
	}
}

int main() {
	glfwSetErrorCallback([](int error, const char* description) {
		std::cerr << "GLFW HATASI (" << error << "): " << description << std::endl;
		});
	Backend::RunEngine();
	return 0;
}