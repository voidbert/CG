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
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include "engine/scene/camera/FreeCamera.hpp"

namespace engine::scene::camera {

FreeCamera::FreeCamera(const glm::vec3 &_position,
                       const glm::vec3 &_lookAt,
                       const glm::vec3 &_up,
                       float _fov,
                       float _near,
                       float _far) :
    Camera(_position, _lookAt, _up, _fov, _near, _far) {

    this->setPosition(this->position);
}

void FreeCamera::setPosition(const glm::vec3 &pos) {
    const glm::vec3 d = glm::normalize(this->lookAt - pos);
    this->yaw = atan2f(d.z, d.x);
    this->pitch = asinf(d.y);

    Camera::setPosition(pos);
}

void FreeCamera::move(const glm::vec3 &v) {
    const float translationSpeed = 100.0f;
    const glm::vec3 d = glm::normalize(this->lookAt - this->position);
    const glm::vec3 right = glm::normalize(glm::cross(d, this->up));

    this->position += glm::mat3(right, this->up, d) * (translationSpeed * v);
    this->update();
}

void FreeCamera::pan(const glm::vec2 &v) {
    const float rotationSpeed = 1.5f;

    this->yaw += rotationSpeed * v.x;
    this->pitch += rotationSpeed * v.y;
    this->update();
}

void FreeCamera::update() {
    this->yaw = glm::mod(this->yaw, glm::two_pi<float>());
    this->pitch =
        glm::clamp(this->pitch, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);

    const glm::vec3 d = glm::vec3(cosf(this->pitch) * cosf(this->yaw),
                                  sinf(this->pitch),
                                  cosf(this->pitch) * sinf(this->yaw));

    this->lookAt = this->position + d;
    Camera::update();
}

}
