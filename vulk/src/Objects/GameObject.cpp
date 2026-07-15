#include "GameObject.h"

	glm::mat4 GameObject::getModelMatrix() const {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);

        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::scale(model, scale);
        return model;
	}

	RenderObject GameObject::getRenderState() {
        RenderObject obj{};
        obj.indexBuffer = indexBuffer;
        obj.indexCount = indexCount;
        obj.firstIndex = 0;
        obj.modelMatrix = getModelMatrix();
        obj.vertexBufferAddress = vertexBufferAddress;
        return obj;
	}


