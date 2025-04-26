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

#include "generator/figures/Cylinder.hpp"

namespace generator::figures {

Cylinder::Cylinder(float radius, float height, int slices, int stacks) {
    this->comment = "cylinder " + std::to_string(radius) + " " + std::to_string(height) + " " +
        std::to_string(slices) + " " + std::to_string(stacks);

    const float sliceStep = glm::two_pi<float>() / slices;
    const float stackStep = height / stacks;

    this->positions.push_back(glm::vec4(0, 0, 0, 1.0f));

    for (int iStack = 0; iStack <= stacks; iStack++) {
        const float y = iStack * stackStep;

        for (int jSlice = 0; jSlice <= slices; jSlice++) {
            const float phi = jSlice * sliceStep;
            const float x = radius * cosf(phi);
            const float z = radius * sinf(phi);

            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    const int topCenterIndex = this->positions.size();
    this->positions.push_back(glm::vec4(0, height, 0, 1.0f));

    for (int jSlice = 0; jSlice < slices; jSlice++) {
        this->faces.push_back(utils::TriangleFace(0, jSlice + 1, jSlice + 2));
    }

    for (int iStack = 0; iStack < stacks; iStack++) {
        for (int jSlice = 0; jSlice < slices; jSlice++) {
            const int currentBottom = iStack * (slices + 1) + jSlice + 1;
            const int nextBottom = currentBottom + 1;
            const int currentTop = currentBottom + slices + 1;
            const int nextTop = currentTop + 1;

            this->faces.push_back(utils::TriangleFace(currentBottom, currentTop, nextTop));
            this->faces.push_back(utils::TriangleFace(currentBottom, nextTop, nextBottom));
        }
    }

    const int topStackStart = topCenterIndex - slices - 1;
    for (int jSlice = 0; jSlice < slices; jSlice++) {
        const int current = topStackStart + jSlice;
        const int next = current + 1;

        this->faces.push_back(utils::TriangleFace(current, topCenterIndex, next));
    }
}

}
