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
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>

#include "generator/figures/Torus.hpp"

namespace generator::figures {

Torus::Torus(float majorRadius, float minorRadius, int slices, int sides) {
    this->comment = "torus " + std::to_string(majorRadius) + " " + std::to_string(minorRadius) +
        " " + std::to_string(slices) + " " + std::to_string(sides);

    const float sliceStep = glm::two_pi<float>() / slices;
    const float sideStep = glm::two_pi<float>() / sides;

    for (int iSlice = 0; iSlice <= slices; iSlice++) {
        const float theta = iSlice * sliceStep;
        const glm::vec3 circleCenter = glm::vec3(cosf(theta), 0.0f, sinf(theta)) * majorRadius;

        for (int jSide = 0; jSide <= sides; jSide++) {
            const float phi = jSide * sideStep;

            const glm::vec3 normal =
                glm::vec3(cosf(theta) * cosf(phi), sinf(phi), sinf(theta) * cosf(phi));
            const glm::vec3 vertex = circleCenter + normal * minorRadius;

            this->positions.push_back(glm::vec4(vertex, 1.0f));
            this->normals.push_back(normal); 

            const float u = static_cast<float>(iSlice) / slices;
            const float v = static_cast<float>(jSide) / sides;
            this->textureCoordinates.push_back(glm::vec2(u, v));
        }
    }

    for (int iSlice = 0; iSlice < slices; iSlice++) {
        for (int jSide = 0; jSide < sides; jSide++) {
            const int currentBottom = iSlice * (sides + 1) + jSide;
            const int nextBottom = currentBottom + 1;
            const int currentTop = currentBottom + (sides + 1);
            const int nextTop = currentTop + 1;

            this->faces.push_back(utils::TriangleFace(
                currentBottom, currentBottom, currentBottom,
                nextTop, nextTop, nextTop,
                currentTop, currentTop, currentTop
            ));

            this->faces.push_back(utils::TriangleFace(
                currentBottom, currentBottom, currentBottom,
                nextBottom, nextBottom, nextBottom,
                nextTop, nextTop, nextTop
            ));
        }
    }
}

}
