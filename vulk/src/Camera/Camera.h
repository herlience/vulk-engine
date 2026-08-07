#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/include/glfw3.h>
#include <imgui.h>

struct Camera {
    glm::vec3 position{ 0.0f, 0.0f, 5.0f };
    glm::vec3 front{ 0.0f, 0.0f, -1.0f };
    glm::vec3 up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 right{ 1.0f, 0.0f, 0.0f };
    glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };

    float yaw{ -90.0f };
    float pitch{ 0.0f };

    float fov{ 70.0f };
    float nearClip{ 0.1f };
    float farClip{ 200.0f };
    float sensitivity{ 0.1f };
    float speed{ 2.5f };

    glm::mat4 viewMatrix{ 1.0f };
    glm::mat4 projectionMatrix{ 1.0f };

    void updateVectors();
};

namespace CameraSystem
{
    inline Camera mainCamera;

    void init(GLFWwindow* window);
    void update(GLFWwindow* window, float deltaTime);

    glm::mat4 getViewProjectionMatrix(float width, float height);
    glm::mat4& getviewmatrix();
    glm::mat4& getprojectionmatrix();

    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
}