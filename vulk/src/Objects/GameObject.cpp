#include "GameObject.h"

	glm::mat4 GameObject::getModelMatrix() const {
        glm::mat4 T = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 R = glm::mat4_cast(rotation);
        glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

        return T * R * S;
	}

	RenderObject GameObject::getRenderState() {
        RenderObject obj{};
        obj.indexBuffer = indexBuffer;
        obj.indexCount = indexCount;
        obj.firstIndex = 0;
        obj.vertexBufferAddress = vertexBufferAddress;
        return obj;
	}


