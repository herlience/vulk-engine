#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <glm/glm.hpp>
#include <array>

struct SwapchainVariables {
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> images;
    VkFormat format;
    VkExtent2D extent;
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;


        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

struct VulkComponents {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugmessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicaldevice;
    VkDevice device;
    SwapchainVariables swapchainvariables;  // vulkTypes'a eklenecek düzenlenecek
    std::vector<VkImageView> imageViews;
    VkPipeline pipeline;
    VkPipelineLayout layout;
    std::vector<VkCommandBuffer> commandBuffers;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    size_t currentFrame;
    int MAX_FRAMES_IN_FLIGHT;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    VkBuffer vertexbuffer;
    VkCommandPool commandpool;
    VkDeviceMemory vertexbuffermemory;
    std::vector<Vertex> vertices;
    PFN_vkCmdSetVertexInputEXT fnCmdSetVertexInputEXT;
};

