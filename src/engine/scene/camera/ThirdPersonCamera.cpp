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

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "engine/scene/camera/ThirdPersonCamera.hpp"

namespace engine::scene::camera {

ThirdPersonCamera::ThirdPersonCamera(const glm::vec3 &_position,
                                     const glm::vec3 &_lookAt,
                                     const glm::vec3 &_up,
                                     float _fov,
                                     float _near,
                                     float _far,
                                     std::unique_ptr<scene::Group> _player) :
    OrbitalCamera(_position, _lookAt, _up, _fov, _near, _far), player(std::move(_player)) {}

void ThirdPersonCamera::move(const glm::vec3 &v) {
    const float walkingSpeed = 2.0f;

    const glm::vec3 d = this->lookAt - this->position;
    const glm::vec3 front = glm::normalize(glm::vec3(d.x, 0.0f, d.z));
    const glm::vec3 right = glm::normalize(glm::cross(front, this->up));

    glm::vec3 motionVector = glm::vec3(0.5 * walkingSpeed);
    if (v.z > 0)
        motionVector.z *= 2.0f;

    this->lookAt += glm::mat3(right, this->up, front) * (v * motionVector);
    this->updateWithMotion();
}

int ThirdPersonCamera::getEntityCount() const {
    return this->player->getEntityCount();
}

void ThirdPersonCamera::updateWithTime(float time) {
    OrbitalCamera::updateWithTime(time);
    this->player->update(this->playerTransform, time);
}

void ThirdPersonCamera::drawSolidColorParts(render::RenderPipelineManager &pipelineManager,
                                            bool showBoundingSpheres,
                                            bool showAnimationLines,
                                            bool showNormals) const {

    return this->player->drawSolidColorParts(pipelineManager,
                                             *this,
                                             this->playerTransform,
                                             showBoundingSpheres,
                                             showAnimationLines,
                                             showNormals);
}

int ThirdPersonCamera::drawShadedParts(render::RenderPipelineManager &pipelineManager,
                                       bool fillPolygons) const {

    return this->player->drawShadedParts(pipelineManager,
                                         *this,
                                         this->playerTransform,
                                         fillPolygons);
}

int ThirdPersonCamera::drawForPicking(render::RenderPipelineManager &pipelineManager,
                                      std::unordered_map<int, std::string> &idToName,
                                      int currentId) const {

    return this->player->drawForPicking(pipelineManager,
                                        *this,
                                        this->playerTransform,
                                        idToName,
                                        currentId);
}

void ThirdPersonCamera::updateWithMotion() {
    OrbitalCamera::updateWithMotion();

    const glm::vec3 d = this->lookAt - this->position;
    const float angle = atan2f(d.x, d.z);
    this->playerTransform = glm::translate(this->lookAt) * glm::rotate(angle, this->up);
}

}
