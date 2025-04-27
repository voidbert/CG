/// Copyright 2025 Ana Oliveira, Humberto Gomes, Mariana Rocha, Sara Lopes
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "engine/scene/camera/CameraController.hpp"

namespace engine::scene::camera {

CameraController::CameraController(Camera &_camera) : camera(_camera), pressedKeys() {}

void CameraController::onUpdate(float time) {
    glm::vec3 move(0.0f);
    glm::vec2 pan(0.0f);
    float zoomFactor = 0.0f;

    for (auto &[key, pressTime] : this->pressedKeys) {
        const float deltaTime = time - pressTime;

        switch (key) {
            case GLFW_KEY_UP:
                pan += glm::vec2(0.0f, 1.0f) * deltaTime;
                break;
            case GLFW_KEY_DOWN:
                pan -= glm::vec2(0.0f, 1.0f) * deltaTime;
                break;
            case GLFW_KEY_LEFT:
                pan -= glm::vec2(1.0f, 0.0f) * deltaTime;
                break;
            case GLFW_KEY_RIGHT:
                pan += glm::vec2(1.0f, 0.0f) * deltaTime;
                break;
            case GLFW_KEY_LEFT_BRACKET: // Plus (PT layout)
                zoomFactor += 1.0f * deltaTime;
                break;
            case GLFW_KEY_SLASH: // Minus (PT layout)
                zoomFactor -= 1.0f * deltaTime;
                break;
            case GLFW_KEY_W:
                move += glm::vec3(0.0f, 0.0f, 1.0f) * deltaTime;
                break;
            case GLFW_KEY_S:
                move -= glm::vec3(0.0f, 0.0f, 1.0f) * deltaTime;
                break;
            case GLFW_KEY_A:
                move -= glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime;
                break;
            case GLFW_KEY_D:
                move += glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime;
                break;
            case GLFW_KEY_SPACE:
                move += glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime;
                break;
            case GLFW_KEY_LEFT_SHIFT:
                move -= glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime;
                break;
            default:
                break;
        }

        pressTime = time;
    }

    this->camera.move(move);
    this->camera.pan(pan);
    this->camera.zoom(zoomFactor);
}

void CameraController::onKeyEvent(int key, int action) {
    const float time = glfwGetTime();

    if (action == GLFW_PRESS) {
        this->pressedKeys[key] = time;
    } else if (action == GLFW_RELEASE) {
        this->onUpdate(time);
        this->pressedKeys.erase(key);
    }
}

}
