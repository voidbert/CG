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

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera/FreeCamera.hpp"

namespace engine::camera {

FreeCamera::FreeCamera(const glm::vec3 &_position,
                       const glm::vec3 &_lookAt,
                       const glm::vec3 &_up,
                       float _fov,
                       float _near,
                       float _far) :
    Camera(_position, _lookAt, _up, _fov, _near, _far), yaw(0.0f), pitch(0.0f) {
    this->front = glm::normalize(_lookAt - _position);
    yaw = atan2f(front.z, front.x);
    pitch = asinf(front.y);

    updateCameraVectors();
}

void FreeCamera::updateCameraVectors() {
    front.x = cosf(pitch) * cosf(yaw);
    front.y = sinf(pitch);
    front.z = cosf(pitch) * sinf(yaw);
    front = glm::normalize(front);

    this->lookAt = this->position + front;
}

void FreeCamera::move(MovementDirection direction, float deltaTime) {
    const float translationSpeed = 5.0f * deltaTime;
    const float rotationSpeed = 1.5f * deltaTime;

    glm::vec3 horizontalFront = glm::normalize(glm::vec3(front.x, front.y, front.z));
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), horizontalFront));

    switch (direction) {
        case MovementDirection::Forward:
            position += horizontalFront * translationSpeed;
            break;
        case MovementDirection::Backward:
            position -= horizontalFront * translationSpeed;
            break;
        case MovementDirection::Left:
            position += right * translationSpeed;
            break;
        case MovementDirection::Right:
            position -= right * translationSpeed;
            break;
        case MovementDirection::Up:
            position += up * translationSpeed;
            break;
        case MovementDirection::Down:
            position -= up * translationSpeed;
            break;
        case MovementDirection::LookLeft:
            yaw -= rotationSpeed;
            updateCameraVectors();
            return;
        case MovementDirection::LookRight:
            yaw += rotationSpeed;
            updateCameraVectors();
            return;
        case MovementDirection::LookUp:
            pitch += rotationSpeed;
            if (pitch > glm::radians(89.0f))
                pitch = glm::radians(89.0f);
            updateCameraVectors();
            return;
        case MovementDirection::LookDown:
            pitch -= rotationSpeed;
            if (pitch < glm::radians(-89.0f))
                pitch = glm::radians(-89.0f);
            updateCameraVectors();
            return;
        default:
            break;
    }

    lookAt = position + front;
}

void FreeCamera::setPosition(const glm::vec3 &newPosition) {
    Camera::setPosition(newPosition);
    this->front = glm::normalize(this->lookAt - newPosition);
    yaw = atan2f(front.z, front.x);
    pitch = asinf(front.y);
}
}
