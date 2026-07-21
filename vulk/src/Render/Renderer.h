#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Vulkan/vulkTypes.h"
#include "RendererTypes.h"
#include "../Camera/Camera.h"
#include "../AssetHandler/AssetHandler.h"
#include <span>
#include <vector>

namespace Renderer
{
    inline std::vector<RenderObject> m_drawlist;

    
    inline void addRenderObject(const RenderObject& obj) {
        m_drawlist.push_back(obj);
    }

    inline void clearDrawlist() {
        m_drawlist.clear();
    }

    void DrawFrame(
        VulkComponents& vulkcomp
    );
    void removeRenderObjectsByIndexBuffer(VkBuffer buffer);
}

