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

#include "engine/scene/camera/Camera.hpp"
#include "engine/scene/camera/OrbitalCamera.hpp"

namespace engine::scene::camera {

OrbitalCamera::OrbitalCamera(const glm::vec3 &_position,
                             const glm::vec3 &_lookAt,
                             const glm::vec3 &_up,
                             float _fov,
                             float _near,
                             float _far) :
    Camera(_position, _lookAt, _up, _fov, _near, _far) {

    this->setPosition(this->position);
}

void OrbitalCamera::setPosition(const glm::vec3 &pos) {
    const glm::vec3 dUnnormalized = pos - this->lookAt;
    this->radius = glm::length(dUnnormalized);

    if (radius > 0.0f) {
        const glm::vec3 d = glm::normalize(dUnnormalized);
        this->azimuth = atan2f(d.z, d.x);
        this->polar = acosf(d.y);
    } else {
        this->azimuth = 0.0f;
        this->polar = 0.0f;
    }

    Camera::setPosition(pos);
}

void OrbitalCamera::pan(const glm::vec2 &v) {
    const float angularSensitivity = 2.5f;
    const glm::vec2 realV = v * angularSensitivity;

    this->azimuth -= realV.x;
    this->polar -= realV.y;
    this->update();
}

void OrbitalCamera::zoom(float factor) {
    const float zoomSpeed = 0.8f;
    this->radius /= 1.0f + factor * zoomSpeed;
    this->update();
}

void OrbitalCamera::update() {
    this->azimuth = glm::mod(this->azimuth, glm::two_pi<float>());
    this->polar = glm::clamp(this->polar, 0.01f, glm::pi<float>() - 0.01f);

    this->position = this->lookAt +
        glm::vec3(this->radius * sinf(this->polar) * cosf(this->azimuth),
                  this->radius * cosf(this->polar),
                  this->radius * sinf(this->polar) * sinf(this->azimuth));

    Camera::update();
}

}
