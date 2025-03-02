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

#include "engine/camera/Camera.hpp"
#include "engine/camera/OrbitalCamera.hpp"

namespace engine::camera {

OrbitalCamera::OrbitalCamera(const glm::vec3 &position,
                             const glm::vec3 &lookAt,
                             const glm::vec3 &up,
                             float fov,
                             float near,
                             float far) :
    Camera(position, lookAt, up, fov, near, far) {
    const glm::vec3 delta = position - this->lookAt;
    radius = glm::length(delta);

    if (radius > 0.0f) {
        const glm::vec3 dir = glm::normalize(delta);
        polar = acos(dir.y);
        azimuth = atan2(dir.z, dir.x);
    } else {
        polar = azimuth = 0.0f;
    }
}

void OrbitalCamera::updatePosition() {
    this->position = this->lookAt +
        glm::vec3(radius * sin(polar) * cos(azimuth),
                  radius * cos(polar),
                  radius * sin(polar) * sin(azimuth));
}

void OrbitalCamera::move(MovementDirection dir, float delta) {
    const float angularSensitivity = 2.5f;
    const float zoomSpeed = 2.0f;

    switch (dir) {
        case MovementDirection::Up:
            polar -= angularSensitivity * delta;
            break;
        case MovementDirection::Down:
            polar += angularSensitivity * delta;
            break;
        case MovementDirection::Left:
            azimuth -= angularSensitivity * delta;
            break;
        case MovementDirection::Right:
            azimuth += angularSensitivity * delta;
            break;
        case MovementDirection::Forward:
            radius -= zoomSpeed * delta;
            break;
        case MovementDirection::Backward:
            radius += zoomSpeed * delta;
            break;
    }

    polar = glm::clamp(polar, 0.01f, glm::pi<float>() - 0.01f);
    azimuth = glm::mod(azimuth, glm::two_pi<float>());
    radius = glm::clamp(radius, 0.5f, 100.0f);
    updatePosition();
}
}
