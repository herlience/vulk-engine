#include "AssetHandler.h"
#include "../Render/Renderer.h"
#include "../Vulkan/Backend/vulkBuffer.h"

namespace AssetHandler {

    static std::vector<Mesh> s_parsedMeshes;
    static std::queue<PendingAsset> s_pendingRenderObjects;
    static std::mutex s_queueMutex;

    void RenderImGui(VulkComponents vulkcomp, VkCommandBuffer cmd) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        ImGuiIO& io = ImGui::GetIO();
        handlemousepicking(
            CameraSystem::getviewmatrix(),
            CameraSystem::getprojectionmatrix(), 
            io.DisplaySize.x, 
            io.DisplaySize.y
        );

        debugpanel();
        assetbrowser(vulkcomp);
        hierarchy(vulkcomp);

        if (selectedModelIndex >= 0 && selectedModelIndex < Renderer::m_drawlist.size())
        {
            GameObject& selectedObje = Renderer::m_gameobjectlist[selectedModelIndex];

            glm::mat4 modelMatrix = selectedObje.getModelMatrix();

            drawgizmo(
                CameraSystem::getviewmatrix(),
                CameraSystem::getprojectionmatrix(),
                modelMatrix
            );

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 skew;
                glm::vec4 perspective;
                glm::quat orientation;

                glm::decompose(modelMatrix, selectedObje.scale, orientation, selectedObje.translation, skew, perspective);
                selectedObje.rotation = glm::degrees(glm::eulerAngles(orientation));
            }
        }

        ImGui::Render();

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
    }

    void debugpanel() {
        ImGui::Begin("debug");

        if (ImGui::Button("show asset")) {
            showAssetBrowser = !showAssetBrowser;
        }

        if (ImGui::Button("show hierarchy")) {
            showHierarchy = !showHierarchy;
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

    static int s_ActiveTexturePickerForObj = -1;

    void hierarchy(VulkComponents vulkcomp) {
        if (!showHierarchy) return;

        ImGui::Begin("hierarchy", &showHierarchy);
        ImGui::Text("Mevcut Ogeler");
        ImGui::Separator();

        for (size_t i = 0; i < Renderer::m_drawlist.size(); i++) {
            auto& obje = Renderer::m_drawlist[i];

            ImGui::Text("Obj %zu | Tex ID: %u", i, obje.textureIndex);
            ImGui::SameLine();

            std::string btnLabel = "Load Texture##" + std::to_string(i);

            if (ImGui::Button(btnLabel.c_str())) {
                if (s_ActiveTexturePickerForObj == static_cast<int>(i)) {
                    s_ActiveTexturePickerForObj = -1; 
                }
                else {
                    s_ActiveTexturePickerForObj = static_cast<int>(i); 
                }
            }

            if (s_ActiveTexturePickerForObj == static_cast<int>(i)) {

                ImGui::Indent(); 
                ImGui::BeginChild(("TexPickerChild##" + std::to_string(i)).c_str(),
                    ImVec2(0, 120), true); 

                ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "--- Texture Secin ---");

                if (cacheTextures.empty()) {
                    cacheTextures = get_tex_files();
                }

                for (size_t t = 0; t < cacheTextures.size(); ++t) {
                    const auto& texPath = cacheTextures[t];

                    std::string fileName = std::filesystem::path(texPath).filename().string();
                    ImGui::Text("%s", fileName.c_str());
                    ImGui::SameLine();

                    std::string selectBtn = "Sec##" + std::to_string(i) + "_" + std::to_string(t);
                    if (ImGui::Button(selectBtn.c_str())) {
                        uint32_t secilentexindex = texturehandler::loadtexturetoengine(texPath.c_str(), vulkcomp);
                        obje.textureIndex = secilentexindex;                        
                        s_ActiveTexturePickerForObj = -1;
                    }
                }

                ImGui::EndChild();
                ImGui::Unindent(); 
            }

            ImGui::Separator();
        }

        ImGui::End();
    }

    void drawgizmo(const glm::mat4& viewMatrix, const glm::mat4& projMatrix, glm::mat4& objectTransform) {
        ImGuizmo::Enable(true);
        ImGuizmo::SetOrthographic(false); 
        ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList()); 

        ImGuiIO& io = ImGui::GetIO();

        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

        static ImGuizmo::OPERATION currentOp = ImGuizmo::TRANSLATE;
        static ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

        if (ImGui::IsKeyPressed(ImGuiKey_W)) currentOp = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(ImGuiKey_E)) currentOp = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(ImGuiKey_R)) currentOp = ImGuizmo::SCALE;

        glm::mat4 correctedProj = projMatrix;
        correctedProj[1][1] *= -1.0f; 

        ImGuizmo::Manipulate(
            glm::value_ptr(viewMatrix),
            glm::value_ptr(correctedProj),
            currentOp,
            currentMode,
            glm::value_ptr(objectTransform)
        );
    }

    void handlemousepicking(const glm::mat4& viewMatrix, const glm::mat4& projMatrix, int width, int height) {
        
        if (ImGui::GetIO().WantCaptureMouse) return;

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            ImVec2 mousePos = ImGui::GetMousePos();

            
            float x = (2.0f * mousePos.x) / static_cast<float>(width) - 1.0f;
            float y = 1.0f - (2.0f * mousePos.y) / static_cast<float>(height); // Y-flip

            
            glm::vec4 rayStartNDC(x, y, -1.0f, 1.0f);
            glm::vec4 rayEndNDC(x, y, 1.0f, 1.0f);

            glm::mat4 invVP = glm::inverse(projMatrix * viewMatrix);
            glm::vec4 rayStartWorld = invVP * rayStartNDC; rayStartWorld /= rayStartWorld.w;
            glm::vec4 rayEndWorld = invVP * rayEndNDC;     rayEndWorld /= rayEndWorld.w;

            glm::vec3 rayDir = glm::normalize(glm::vec3(rayEndWorld - rayStartWorld));
            glm::vec3 rayOrigin = glm::vec3(rayStartWorld);

            
            int closestIndex = -1;
            float minDistance = 999999.0f;

            for (size_t i = 0; i < Renderer::m_gameobjectlist.size(); ++i) {
                glm::vec3 objPos = Renderer::m_gameobjectlist[i].translation;

                
                glm::vec3 lineToObj = objPos - rayOrigin;
                float projection = glm::dot(lineToObj, rayDir);

                if (projection > 0.0f) { 
                    glm::vec3 closestPoint = rayOrigin + rayDir * projection;
                    float distToRay = glm::length(objPos - closestPoint);

                    
                    if (distToRay < 1.5f && projection < minDistance) {
                        minDistance = projection;
                        closestIndex = static_cast<int>(i);
                    }
                }
            }

            if (closestIndex != -1) {
                selectedModelIndex = closestIndex; 
            }
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
                << " | Vertices: " << static_cast<uint32_t>(outVertices.size())
                << " | Indices: " << static_cast<uint32_t>(outIndices.size()) << std::endl;

        RenderObject newObj;
        newObj.indexBuffer = gpuBuffers.indexBuffer.buffer;
        newObj.vertexBufferAddress = gpuBuffers.vertexBufferAddress;
        newObj.indexCount = static_cast<uint32_t>(outIndices.size());
        newObj.firstIndex = 0;

        GameObject newgameobj;
        newgameobj.indexCount = static_cast<uint32_t>(outIndices.size());

        PendingAsset assets = { newObj, newgameobj };

        {
            std::lock_guard<std::mutex> lock(s_queueMutex);
            s_pendingRenderObjects.push(assets);
        }

    }

    void syncpendingobjects() {
        std::lock_guard<std::mutex> lock(AssetHandler::s_queueMutex);

        while (!AssetHandler::s_pendingRenderObjects.empty()) {
            PendingAsset readyObj = AssetHandler::s_pendingRenderObjects.front();
            AssetHandler::s_pendingRenderObjects.pop();

            Renderer::addRenderObject(readyObj.renderobj);
            uint32_t meshIndex = static_cast<uint32_t>(Renderer::m_drawlist.size() - 1);

            readyObj.gameobj.renderObjectIndex = meshIndex;
            Renderer::m_gameobjectlist.push_back(readyObj.gameobj);
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

    std::vector<std::string> get_tex_files(const std::string folderpath) {
        std::vector<std::string> texfiles;

        if (!std::filesystem::exists(folderpath) || !std::filesystem::is_directory(folderpath)) {
            std::cerr << "textures klasorunu bulamadim" << std::endl;
            return texfiles;
        }

        for (const auto& entry : std::filesystem::directory_iterator(folderpath)) {
            if (entry.is_regular_file()) {
                auto ext = entry.path().extension().string();
                
                if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
                    texfiles.push_back(entry.path().generic_string());
                }
            
            }
        }

        return texfiles;
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