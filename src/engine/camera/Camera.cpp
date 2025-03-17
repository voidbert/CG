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

#include "engine/camera/Camera.hpp"

namespace engine::camera {

Camera::Camera(const glm::vec3 &pos,
               const glm::vec3 &target,
               const glm::vec3 &upDir,
               float fovAngle,
               float near,
               float far) :
    position(pos), lookAt(target), up(upDir), fov(fovAngle), nearPlane(near), farPlane(far) {}

void Camera::setPosition(const glm::vec3 &pos) {
    this->position = pos;
}

glm::mat4 Camera::getCameraMatrix(float aspectRatio) const {
    glm::mat4 view = glm::lookAt(this->position, this->lookAt, this->up);
    glm::mat4 projection =
        glm::perspective(glm::radians(this->fov), aspectRatio, this->nearPlane, this->farPlane);
    return projection * view;
}
glm::vec3 Camera::getPosition() const {
    return this->position;
}

}
