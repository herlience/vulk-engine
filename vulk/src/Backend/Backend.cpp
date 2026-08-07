#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "Backend.h"
#include "../Vulkan/vulkBackend.h"
#include "../Render/ObjectRenderer.h"
#include "../Render/Renderer.h"
#include "../Camera/Camera.h"
#include "../Objects/GameObject.h"
#include "../AssetHandler/AssetHandler.h"
#include "../Render/imguiinit.h"

#ifdef _WIN32
#include <windows.h>

void EnableVTMode() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

namespace Backend {
    void RunEngine() {
        vulkBackend::InitVulk();
        VulkComponents vulkcomp = vulkBackend::getvulkcomp();
        GLFWwindow* window = vulkBackend::getwindow();
        QueueFamilyIndices familyindex = vulkBackend::getfamilyindex();
        Imgui::init(vulkcomp, familyindex, window);

        
        float lastFrame = 0.0f;

        while (!vulkanWindow::ShouldClose(window)) {
            vulkanWindow::PollEvents();

            
            float currentFrame = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
            CameraSystem::update(window, deltaTime);

            glm::mat4 viewProj = CameraSystem::getViewProjectionMatrix(
                static_cast<float>(vulkcomp.swapchainvariables.extent.width),
                static_cast<float>(vulkcomp.swapchainvariables.extent.height)
            );

            Renderer::DrawFrame(vulkcomp);
        }

        vulkBackend::WaitIdle();
        Imgui::destroy();
        vulkBackend::DestroyVulk();
    }
}

int main() {
#ifdef _WIN32
    EnableVTMode();
#endif
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW HATASI (" << error << "): " << description << std::endl;
        });
    Backend::RunEngine();
    return 0;
}