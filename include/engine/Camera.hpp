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

namespace engine {

class Camera {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float pitch, yaw;

    void updateVectors();

public:
    Camera();

    void move(const glm::vec3 &direction, float deltaTime);
    void rotate(float deltaYaw, float deltaPitch);

    glm::mat4 getCameraMatrix(float aspectRatio) const;
};

}
