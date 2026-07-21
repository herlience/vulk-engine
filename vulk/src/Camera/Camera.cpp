#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CameraSystem
{
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    void CameraSystem::updateViewMatrix() {
        m_viewMatrix = glm::lookAt(position, target, up);
    }
    void CameraSystem::updateProjectionMatrix(float width, float height) {
        float aspect = width / height;
        m_projectionMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 200.0f);
        m_projectionMatrix[1][1] *= -1.0f;
    }
    glm::mat4 CameraSystem::getViewProjectionMatrix() {
        return m_projectionMatrix * m_viewMatrix;
    }
}