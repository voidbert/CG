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

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/Camera.hpp"

namespace engine {

Camera::Camera() :
    position(0.0f, 0.0f, 3.0f),
    front(0.0f, 0.0f, -1.0f),
    up(0.0f, 1.0f, 0.0f),
    pitch(0.0f),
    yaw(-glm::half_pi<float>()) {
    updateVectors();
}

void Camera::move(const glm::vec3 &direction, float deltaTime) {
    float speed = 2.5f * deltaTime;
    position += direction * speed;
}

void Camera::rotate(float deltaYaw, float deltaPitch) {
    yaw += deltaYaw;
    pitch += deltaPitch;

    if (pitch > glm::radians(89.0f))
        pitch = glm::radians(89.0f);
    if (pitch < glm::radians(-89.0f))
        pitch = glm::radians(-89.0f);

    updateVectors();
}

glm::mat4 Camera::getCameraMatrix(float aspectRatio) const {
    glm::mat4 view = glm::lookAt(position, position + front, up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    return projection * view;
}

void Camera::updateVectors() {
    glm::vec3 direction;
    direction.x = cos(yaw) * cos(pitch);
    direction.y = sin(pitch);
    direction.z = sin(yaw) * cos(pitch);

    front = glm::normalize(direction);
}

}
