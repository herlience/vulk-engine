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
		Imgui::init(vulkcomp, familyindex,window);

		

		while (!vulkanWindow::ShouldClose(vulkBackend::getwindow())) {
			vulkanWindow::PollEvents();
			Renderer::DrawFrame();
		}

		vulkBackend::WaitIdle();
		vulkBackend::DestroyVulk();
	}
}

int main() {
	Backend::RunEngine();
	return 0;
}