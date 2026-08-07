#include "Camera.h"

void Camera::updateVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

namespace CameraSystem
{
    static float lastX = 1200.0f / 2.0f;
    static float lastY = 800.0f / 2.0f;
    static bool firstMouse = true;

    static bool isCursorLocked = true;
    static bool escapeWasPressed = false;

    void init(GLFWwindow* window) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouseCallback);
        mainCamera.updateVectors();
    }

    void mouseCallback(GLFWwindow* window, double xposin, double yposin) {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse || !isCursorLocked) {
            
            lastX = static_cast<float>(xposin);
            lastY = static_cast<float>(yposin);
            return;
        }

        float xpos = static_cast<float>(xposin);
        float ypos = static_cast<float>(yposin);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; 
        lastX = xpos;
        lastY = ypos;

        xoffset *= mainCamera.sensitivity;
        yoffset *= mainCamera.sensitivity;

        mainCamera.yaw += xoffset;
        mainCamera.pitch += yoffset;

        if (mainCamera.pitch > 89.0f)  mainCamera.pitch = 89.0f;
        if (mainCamera.pitch < -89.0f) mainCamera.pitch = -89.0f;

        
        mainCamera.updateVectors();
    }

    
    void update(GLFWwindow* window, float deltaTime) {
        bool escapePressed = (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);

        
        if (escapePressed && !escapeWasPressed) {
            isCursorLocked = !isCursorLocked;

            if (isCursorLocked) {
                
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                firstMouse = true; 
            }
            else {
                
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        escapeWasPressed = escapePressed;

        float velocity = mainCamera.speed * deltaTime;

        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            mainCamera.position += mainCamera.front * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            mainCamera.position -= mainCamera.front * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            mainCamera.position -= mainCamera.right * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            mainCamera.position += mainCamera.right * velocity;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            mainCamera.position += mainCamera.worldUp * velocity;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            mainCamera.position -= mainCamera.worldUp * velocity;
    }

    glm::mat4 getViewProjectionMatrix(float width, float height) {
        
        mainCamera.viewMatrix = glm::lookAt(mainCamera.position, mainCamera.position + mainCamera.front, mainCamera.up);

        
        float aspect = width / height;
        mainCamera.projectionMatrix = glm::perspective(glm::radians(mainCamera.fov), aspect, mainCamera.nearClip, mainCamera.farClip);
        mainCamera.projectionMatrix[1][1] *= -1.0f; 

        return mainCamera.projectionMatrix * mainCamera.viewMatrix;
    }

    glm::mat4& getviewmatrix() {
        return mainCamera.viewMatrix;
    }
    glm::mat4& getprojectionmatrix() {
        return mainCamera.projectionMatrix;
    }
}