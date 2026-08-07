#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <imgui/imgui.h>
#include <imgui/imgui_impl_vulkan.h>
#include <imgui/imgui_impl_glfw.h>

#include <imguizmo/ImGuizmo.h>

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <mutex>

#include <TinyOBJ/tiny_obj_loader.h>
#include <GLFW/include/glfw3.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "../Render/RendererTypes.h"
#include "../Render/Renderer.h"
#include "../Render/ObjectRenderer.h"
#include "../Objects/GameObject.h"
#include "../Vulkan/vulkTypes.h"
#include "../Vulkan/vulkBackend.h"
#include "texturehandler.h"

struct PendingAsset {
	RenderObject renderobj;
	GameObject gameobj;
};

namespace AssetHandler
{
	std::vector<std::string> get_obj_files(const std::string& folderpath = "res");
	std::vector<std::string> get_tex_files(const std::string folderpath = "res/Textures");

	inline std::vector<std::string> cacheModels = get_obj_files();
	inline std::vector<std::string> cacheTextures = get_tex_files();

	static int selectedModelIndex = -1;

	static bool showAssetBrowser = false;
	static bool showHierarchy = false;
	static bool showTextureHierarchy = false;

	void syncpendingobjects();

	void RenderImGui(VulkComponents vulkcomp, VkCommandBuffer cmd);
	
	void debugpanel();
	void assetbrowser(VulkComponents vulkcomp);
	void hierarchy(VulkComponents vulkcomp);

	void drawgizmo(const glm::mat4& viewMatrix, const glm::mat4& projMatrix, glm::mat4& objectTransform);
	void handlemousepicking(const glm::mat4& viewMatrix, const glm::mat4& projMatrix, int width, int height);

	void loadobject(
		const std::string& path,
		VkDevice device,
		VmaAllocator allocator,
		VkCommandPool commandPool,
		VkQueue graphicsQueue
	);
	void cleanup(VmaAllocator allocator);
	void unloadMesh(size_t index, VmaAllocator allocator); 
};

