#include "AssetHandler.h"
#include "../Render/Renderer.h"
#include "../Vulkan/Backend/vulkBuffer.h"

namespace AssetHandler {

    static std::vector<Mesh> s_parsedMeshes;
    static std::queue<RenderObject> s_pendingRenderObjects;
    static std::mutex s_queueMutex;

    void RenderImGui(VulkComponents vulkcomp, VkCommandBuffer cmd) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        debugpanel();
        assetbrowser(vulkcomp);

        ImGui::Render();

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
    }

    void debugpanel() {
        ImGui::Begin("debug");

        if (ImGui::Button("show asset")) {
            showAssetBrowser = !showAssetBrowser;
        }

        ImGui::End();
    }

    void assetbrowser(VulkComponents vulkcomp) {
        if (showAssetBrowser) {
            if (cacheModels.empty()) {
                cacheModels = get_obj_files();
            }

            ImGui::Begin("asset browser", &showAssetBrowser);

            for (size_t i = 0; i < cacheModels.size(); ++i) {
                const auto& obje = cacheModels[i];

                ImGui::Text("%s", obje.c_str());
                ImGui::SameLine();

                std::string btnLabel = std::string("Load##") + std::to_string(i);
                if (ImGui::Button(btnLabel.c_str())) {
                    selectedModelIndex = static_cast<int>(i);
                    loadobject(obje, vulkcomp.device, vulkcomp.allocator, vulkcomp.commandpool, vulkcomp.graphicsQueue);
                }
            }

            ImGui::End();
        }
    }

    void loadobject(
        const std::string& path,
        VkDevice device,
        VmaAllocator allocator,
        VkCommandPool commandPool,
        VkQueue graphicsQueue
    ) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;

        std::filesystem::path objPath(path);
        std::string materialFolder = objPath.parent_path().string() + "/";

        
        if (objPath.parent_path().empty()) {
            materialFolder = "./";
        }

        bool ret = tinyobj::LoadObj(
            &attrib,
            &shapes,
            &materials,
            &err,
            path.c_str(),
            materialFolder.c_str()
        );

        if (!err.empty()) {
            std::cerr << "TinyObj error: " << err << std::endl;
        }
        if (!ret) {
            std::cerr << "Failed to load/parse .obj file: " << path << std::endl;
            return;
        }

        struct IndexTriplet { int v, vt, vn; };
        struct TripletHash {
            size_t operator()(IndexTriplet const& t) const noexcept {
                size_t h = 1469598103934665603ull;
                h ^= std::hash<int>()(t.v) + 0x9e3779b97f4a7c15 + (h << 6) + (h >> 2);
                h ^= std::hash<int>()(t.vt) + 0x9e3779b97f4a7c15 + (h << 6) + (h >> 2);
                h ^= std::hash<int>()(t.vn) + 0x9e3779b97f4a7c15 + (h << 6) + (h >> 2);
                return h;
            }
        };
        struct TripletEq {
            bool operator()(IndexTriplet const& a, IndexTriplet const& b) const noexcept {
                return a.v == b.v && a.vt == b.vt && a.vn == b.vn;
            }
        };

        std::unordered_map<IndexTriplet, uint32_t, TripletHash, TripletEq> indexMap;

        std::vector<Vertex> outVertices;
        std::vector<uint32_t> outIndices;

        for (const auto& shape : shapes) {
            size_t index_offset = 0;
            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
                int fv = shape.mesh.num_face_vertices[f];
                for (int v = 0; v < fv; ++v) {
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                    IndexTriplet trip{ idx.vertex_index, idx.texcoord_index, idx.normal_index };

                    auto it = indexMap.find(trip);
                    if (it != indexMap.end()) {
                        outIndices.push_back(it->second);
                    }
                    else {
                        Vertex vert{};

                        if (trip.v >= 0) {
                            vert.pos.x = attrib.vertices[3 * trip.v + 0];
                            vert.pos.y = attrib.vertices[3 * trip.v + 1];
                            vert.pos.z = attrib.vertices[3 * trip.v + 2];
                        }
                        else {
                            vert.pos = glm::vec3(0.0f);
                        }

                        if (trip.vn >= 0) {
                            vert.normal.x = attrib.normals[3 * trip.vn + 0];
                            vert.normal.y = attrib.normals[3 * trip.vn + 1];
                            vert.normal.z = attrib.normals[3 * trip.vn + 2];
                        }
                        else {
                            vert.normal = glm::vec3(0.0f, 0.0f, 1.0f);
                        }

                        if (trip.vt >= 0) {
                            vert.uv_x = attrib.texcoords[2 * trip.vt + 0];
                            vert.uv_y = attrib.texcoords[2 * trip.vt + 1];
                        }
                        else {
                            vert.uv_x = vert.uv_y = 0.0f;
                        }

                        vert.color = glm::vec4(1.0f);

                        uint32_t newIndex = static_cast<uint32_t>(outVertices.size());
                        outVertices.push_back(vert);
                        outIndices.push_back(newIndex);
                        indexMap[trip] = newIndex;
                    }
                }
                index_offset += fv;
            }
        }

        GPUMeshBuffers gpuBuffers = ObjectRenderer::uploadMesh(
            device,
            std::span<uint32_t>(outIndices),
            std::span<Vertex>(outVertices),
            allocator,
            commandPool,
            graphicsQueue
        );

     
        Mesh newMesh;
        newMesh.buffers = gpuBuffers;
        newMesh.indexcount = static_cast<uint32_t>(outIndices.size());
        newMesh.vertexcount = static_cast<uint32_t>(outVertices.size());

        s_parsedMeshes.push_back(newMesh);
        std::cout << " Mesh cached "
                << " | Vertices: " << newMesh.vertexcount 
                << " | Indices: " << newMesh.indexcount << std::endl;

        RenderObject newObj;
        newObj.indexBuffer = gpuBuffers.indexBuffer.buffer;
        newObj.vertexBufferAddress = gpuBuffers.vertexBufferAddress;
        newObj.indexCount = static_cast<uint32_t>(outIndices.size());
        newObj.firstIndex = 0;
        newObj.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        {
            std::lock_guard<std::mutex> lock(s_queueMutex);
            s_pendingRenderObjects.push(newObj);
        }

    }

    void syncpendingobjects() {
        std::lock_guard<std::mutex> lock(AssetHandler::s_queueMutex);

        while (!AssetHandler::s_pendingRenderObjects.empty()) {
            RenderObject readyObj = AssetHandler::s_pendingRenderObjects.front();
            AssetHandler::s_pendingRenderObjects.pop();

            Renderer::addRenderObject(readyObj);
        }
    }

    std::vector<std::string> get_obj_files(const std::string& folderPath) {
        std::vector<std::string> objFiles;

        if (!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath)) {
            std::cerr << "res klasoru bulunamadi!" << std::endl;
            return objFiles;
        }

        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".obj") {
                objFiles.push_back(entry.path().string());
            }
        }
        return objFiles;
    }

    void cleanup(VmaAllocator allocator) {
        for (size_t i = 0; i < s_parsedMeshes.size(); ++i) {
            auto& mesh = s_parsedMeshes[i];

            if (mesh.buffers.indexBuffer.buffer != VK_NULL_HANDLE) {
                vulkBuffer::destroy_buffer(mesh.buffers.indexBuffer, allocator);
            }

            if (mesh.buffers.vertexBuffer.buffer != VK_NULL_HANDLE) {
                vulkBuffer::destroy_buffer(mesh.buffers.vertexBuffer, allocator);
            }
        }

        s_parsedMeshes.clear();
        Renderer::clearDrawlist();
    }

    void unloadMesh(size_t index, VmaAllocator allocator) {
        if (index >= s_parsedMeshes.size()) {
            return;
        }

        Mesh mesh = s_parsedMeshes[index];

        Renderer::removeRenderObjectsByIndexBuffer(mesh.buffers.indexBuffer.buffer);

        if (mesh.buffers.indexBuffer.buffer != VK_NULL_HANDLE) {
            vulkBuffer::destroy_buffer(mesh.buffers.indexBuffer, allocator);
        }

        if (mesh.buffers.vertexBuffer.buffer != VK_NULL_HANDLE) {
            vulkBuffer::destroy_buffer(mesh.buffers.vertexBuffer, allocator);
        }

        s_parsedMeshes.erase(s_parsedMeshes.begin() + index);
    }
}