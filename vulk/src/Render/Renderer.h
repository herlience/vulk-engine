#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Vulkan/vulkTypes.h"
#include "RendererTypes.h"
#include "../Camera/Camera.h"
#include <span>
#include <vector>

namespace Renderer
{
    inline std::vector<RenderObject> m_drawlist;

    void addRenderObject(const RenderObject& obj) {
        m_drawlist.push_back(obj);
    }

    void clearDrawlist() {
        m_drawlist.clear();
    }

    void DrawFrame(
        VulkComponents& vulkcomp,
        VkSwapchainKHR swapchain,
        const std::vector<VkImage>& images,
        VkFormat swapchainFormat,
        VkExtent2D extent,
        std::span<Vertex> vertices,
        GPUMeshBuffers meshbuffers,
        Camera& camera
    );

}

