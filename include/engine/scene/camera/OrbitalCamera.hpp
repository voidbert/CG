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

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "engine/scene/camera/Camera.hpp"

namespace engine::scene::camera {

class OrbitalCamera : public Camera {
protected:
    float radius, azimuth, polar;

public:
    OrbitalCamera(const glm::vec3 &_position,
                  const glm::vec3 &_lookAt,
                  const glm::vec3 &_up,
                  float _fov,
                  float _near,
                  float _far);

    void setPosition(const glm::vec3 &pos) override;

    void pan(const glm::vec2 &v) override;
    void zoom(float factor) override;

protected:
    void update() override;
};

}
