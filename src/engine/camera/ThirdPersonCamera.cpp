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

#include <glm/gtx/transform.hpp>

#include "engine/camera/ThirdPersonCamera.hpp"

namespace engine::camera {

ThirdPersonCamera::ThirdPersonCamera(const glm::vec3 &_position,
                                     const glm::vec3 &_lookAt,
                                     const glm::vec3 &_up,
                                     float _fov,
                                     float _near,
                                     float _far,
                                     std::unique_ptr<scene::Group> _player) :
    Camera(_position, _lookAt, _up, _fov, _near, _far), player(std::move(_player)) {

    const glm::vec3 delta = _position - this->lookAt;
    this->radius = glm::length(delta);

    if (radius > 0.0f) {
        const glm::vec3 dir = glm::normalize(delta);
        azimuth = atan2f(dir.z, dir.x);
        polar = acosf(dir.y);
    } else {
        polar = azimuth = 0.0f;
    }
}

void ThirdPersonCamera::setPosition(const glm::vec3 &pos) {
    const glm::vec3 eyeTranslation = this->position - this->lookAt;
    this->lookAt = pos;
    Camera::setPosition(this->lookAt + eyeTranslation);
}

void ThirdPersonCamera::move(MovementDirection dir, float delta) {
    const float angularSensitivity = 2.5f;
    const float zoomSpeed = 2.0f;
    const float walkingSpeed = 2.0f;

    const glm::vec3 front = this->lookAt - this->position;
    const glm::vec3 horizontalFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    const glm::vec3 right = glm::normalize(glm::cross(this->up, horizontalFront));

    switch (dir) {
        case MovementDirection::Up:
            this->lookAt += horizontalFront * delta * walkingSpeed;
            break;
        case MovementDirection::Down:
            this->lookAt -= 0.5f * horizontalFront * delta * walkingSpeed;
            break;
        case MovementDirection::Right:
            this->lookAt -= 0.5f * right * delta * walkingSpeed;
            break;
        case MovementDirection::Left:
            this->lookAt += 0.5f * right * delta * walkingSpeed;
            break;
        case MovementDirection::LookUp:
            this->polar -= angularSensitivity * delta;
            break;
        case MovementDirection::LookDown:
            this->polar += angularSensitivity * delta;
            break;
        case MovementDirection::LookLeft:
            this->azimuth += angularSensitivity * delta;
            break;
        case MovementDirection::LookRight:
            this->azimuth -= angularSensitivity * delta;
            break;
        case MovementDirection::Forward:
            this->radius -= zoomSpeed * delta;
            break;
        case MovementDirection::Backward:
            this->radius += zoomSpeed * delta;
            break;
        default:
            break;
    }

    azimuth = glm::mod(azimuth, glm::two_pi<float>());
    polar = glm::clamp(polar, 0.01f, glm::pi<float>() - 0.01f);
    radius = glm::clamp(radius, 0.5f, 100.0f);

    this->updatePosition();
    this->updateCameraMatrix();
    this->updateViewFrustum();
}

void ThirdPersonCamera::updatePosition() {
    this->position = this->lookAt +
        glm::vec3(this->radius * sinf(this->polar) * cosf(this->azimuth),
                  this->radius * cosf(this->polar),
                  this->radius * sinf(this->polar) * sinf(this->azimuth));
}

void ThirdPersonCamera::draw(const render::RenderPipeline &pipeline, bool drawBoundingSpheres) {
    const glm::vec3 d = this->lookAt - this->position;
    const float angle = atan2f(d.x, d.z);
    const glm::mat4 worldTransform = glm::translate(this->lookAt) * glm::rotate(angle, this->up);

    this->player->updateBoundingSphere(worldTransform);
    this->player->draw(pipeline,
                       *this,
                       this->getCameraMatrix() * worldTransform,
                       drawBoundingSpheres);
}

}
