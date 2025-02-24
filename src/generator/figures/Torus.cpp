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
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "generator/figures/Torus.hpp"

namespace generator::figures {

Torus::Torus(float majorRadius, float minorRadius, int slices, int stacks) {
    for (int i = 0; i <= slices; i++) {
        float theta = i * 2 * M_PI / slices;
        glm::vec3 circleCenter = glm::vec3(glm::cos(theta), 0.0f, glm::sin(theta)) * majorRadius;

        for (int j = 0; j <= stacks; j++) {
            float phi = j * 2 * M_PI / stacks;
            glm::vec3 normal = glm::vec3(glm::cos(theta) * glm::cos(phi),
                                         glm::sin(phi),
                                         glm::sin(theta) * glm::cos(phi));
            glm::vec3 vertex = -circleCenter + normal * minorRadius;
            this->positions.push_back(glm::vec4(vertex, 1.0f));
        }
    }

    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            int idx = i * (stacks + 1) + j;
            int nextIdx = ((i + 1) % slices) * (stacks + 1) + j;

            this->faces.push_back(utils::TriangleFace(idx, nextIdx, idx + 1));
            this->faces.push_back(utils::TriangleFace(nextIdx, nextIdx + 1, idx + 1));
        }
    }
}

}
