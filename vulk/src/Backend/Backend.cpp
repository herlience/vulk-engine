#include "Backend.h"
#include "../Vulkan/vulkBackend.h"
namespace Backend {
	void RunEngine() {
		vulkBackend::InitVulk();

		while (!vulkanWindow::ShouldClose(vulkBackend::getwindow())) {
			vulkanWindow::PollEvents();
			vulkBackend::RenderSingleFrame();
		}

		vulkBackend::WaitIdle();
		vulkBackend::DestroyVulk();
	}
}

int main() {
	Backend::RunEngine();
	return 0;
}