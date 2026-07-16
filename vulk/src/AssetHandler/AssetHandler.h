#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_vulkan.h>
#include <imgui/imgui_impl_glfw.h>

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <mutex>

#include <TinyOBJ/tiny_obj_loader.h>

#include "../Render/RendererTypes.h"
#include "../Render/Renderer.h"
#include "../Render/ObjectRenderer.h"
#include "../Vulkan/vulkTypes.h"

namespace AssetHandler
{
	std::vector<std::string> cacheModels = get_obj_files();
	static int selectedModelIndex = -1;
	static bool showAssetBrowser = false;


	void RenderImGui(VulkComponents vulkcomp);
	
	void debugpanel();
	void assetbrowser(VulkComponents vulkcomp);

	void loadobject(
		const std::string& path,
		VkDevice device,
		VmaAllocator allocator,
		VkCommandPool commandPool,
		VkQueue graphicsQueue
	);


	std::vector<std::string> get_obj_files(const std::string& folderpath = "res");
};

