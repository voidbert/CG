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
#include "engine/camera/Camera.hpp"

namespace engine::camera {

class ThirdPersonCamera : public Camera {
private:
    float azimuth;
    float polar;
    float radius;

    void updatePosition();

public:
    ThirdPersonCamera(const glm::vec3 &_position,
                  const glm::vec3 &_lookAt,
                  const glm::vec3 &_up,
                  float _fov,
                  float _near,
                  float _far);

    void move(MovementDirection direction, float deltaTime) override;
};

}
