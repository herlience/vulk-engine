#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
    glm::vec3 position{ 0.0f, 0.0f, 3.0f };  
    glm::vec3 front{ 0.0f, 0.0f, -1.0f };    
    glm::vec3 up{ 0.0f, 1.0f, 0.0f };         
    glm::vec3 right{ 1.0f, 0.0f, 0.0f };       

    float yaw{ -90.0f };  
    float pitch{ 0.0f };  

    float fov{ 70.0f };       
    float nearClip{ 0.1f };   // En yakýn çizim mesafesi
    float farClip{ 100.0f };  // En uzak çizim mesafesi

    glm::mat4 viewMatrix{ 1.0f };
    glm::mat4 projectionMatrix{ 1.0f };
};

namespace CameraSystem
{
    inline glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
    inline glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    inline glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    void updateViewMatrix();
    void updateProjectionMatrix(float width, float height);
    glm::mat4 getViewProjectionMatrix();
}

