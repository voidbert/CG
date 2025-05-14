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
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "generator/figures/Cone.hpp"

namespace generator::figures {

Cone::Cone(float radius, float height, int slices, int stacks) {
    this->comment = "cone " + std::to_string(radius) + " " + std::to_string(height) + " " +
        std::to_string(slices) + " " + std::to_string(stacks);

    const float stackStep = height / stacks;
    const float sliceStep = glm::two_pi<float>() / slices;

    // Generate base
    this->positions.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    this->textureCoordinates.push_back(glm::vec2(0.5f, 0.5f));
    this->normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    for (int jSlice = 0; jSlice <= slices; jSlice++) {
        const float angle = jSlice * sliceStep;
        const float x = radius * cosf(angle);
        const float z = radius * sinf(angle);
        this->positions.push_back(glm::vec4(x, 0.0f, z, 1.0f));

        const float s = 0.5 + 0.5 * cosf(angle);
        const float t = 0.5 + 0.5 * sinf(angle);
        this->textureCoordinates.push_back(glm::vec2(s, t));
    }

    // Generate stacks
    for (int iStack = 0; iStack <= stacks; iStack++) {
        const float y = iStack * stackStep;
        const float stackRadius = ((height - y) * radius) / height;
        const float v = static_cast<float>(iStack) / stacks;

        for (int jSlice = 0; jSlice <= slices; jSlice++) {
            const float angle = jSlice * sliceStep;
            const float x = stackRadius * cosf(angle);
            const float z = stackRadius * sinf(angle);
            this->positions.push_back(glm::vec4(x, y, z, 1.0f));

            const float u = static_cast<float>(jSlice) / slices;
            this->textureCoordinates.push_back(glm::vec2(u, v));
        }
    }

    for (int jSlice = 0; jSlice < slices; ++jSlice) {
        const float angle = jSlice * sliceStep;
        const float nx = cosf(angle + sliceStep / 2);
        const float ny = radius / height;
        const float nz = sinf(angle + sliceStep / 2);
        this->normals.push_back(glm::normalize(glm::vec3(nx, ny, nz)));
    }

    // Triangulate base
    for (int jSlice = 0; jSlice < slices; jSlice++) {
        this->faces.push_back(
            utils::TriangleFace(0, 0, 0, jSlice + 1, jSlice + 1, 0, jSlice + 2, jSlice + 2, 0));
    }

    // Triangulate stacks
    for (int iStack = 0; iStack < stacks; iStack++) {
        for (int jSlice = 0; jSlice < slices; jSlice++) {
            const int currentBottom = iStack * (slices + 1) + jSlice + slices + 2;
            const int nextBottom = currentBottom + 1;
            const int currentTop = currentBottom + slices + 1;
            const int nextTop = currentTop + 1;

            const int faceNormal = (jSlice) % slices + 1;

            this->faces.push_back(utils::TriangleFace(currentBottom,
                                                      currentBottom,
                                                      faceNormal,
                                                      currentTop,
                                                      currentTop,
                                                      faceNormal,
                                                      nextTop,
                                                      nextTop,
                                                      faceNormal));
            this->faces.push_back(utils::TriangleFace(currentBottom,
                                                      currentBottom,
                                                      faceNormal,
                                                      nextTop,
                                                      nextTop,
                                                      faceNormal,
                                                      nextBottom,
                                                      nextBottom,
                                                      faceNormal));
        }
    }
}

}
