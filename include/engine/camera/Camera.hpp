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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine::camera {

enum class MovementDirection {
    Up,
    Down,
    Left,
    Right,
    Forward,
    Backward
};

class Camera {
protected:
    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 up;
    float fov;
    float nearPlane;
    float farPlane;

public:
    Camera(const glm::vec3 &position,
           const glm::vec3 &lookAt,
           const glm::vec3 &up,
           float fov,
           float nearPlane,
           float farPlane);
    virtual ~Camera() = default;

    virtual void move(MovementDirection direction, float deltaTime) = 0;
    glm::mat4 getCameraMatrix(float aspectRatio) const;
};

}
