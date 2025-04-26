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

#include "generator/figures/Cone.hpp"

namespace generator::figures {

Cone::Cone(float radius, float height, int slices, int stacks) {
    this->comment = "cone " + std::to_string(radius) + " " + std::to_string(height) + " " +
        std::to_string(slices) + " " + std::to_string(stacks);

    const float stackStep = height / stacks;
    const float sliceStep = glm::two_pi<float>() / slices;

    this->positions.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    for (int iStack = 0; iStack < stacks; iStack++) {
        const float y = iStack * stackStep;
        const float stackRadius = ((height - y) * radius) / height;

        for (int jSlice = 0; jSlice <= slices; jSlice++) {
            const float angle = jSlice * sliceStep;
            const float x = stackRadius * cosf(angle);
            const float z = stackRadius * sinf(angle);
            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    const int topVertexIndex = this->positions.size();
    this->positions.push_back(glm::vec4(0.0f, height, 0.0f, 1.0f));

    for (int jSlice = 0; jSlice < slices; jSlice++) {
        this->faces.push_back(utils::TriangleFace(0, jSlice + 1, jSlice + 2));
    }

    for (int iStack = 0; iStack < stacks - 1; iStack++) {
        for (int jSlice = 0; jSlice < slices; jSlice++) {
            const int currentBottom = iStack * (slices + 1) + jSlice + 1;
            const int nextBottom = currentBottom + 1;
            const int currentTop = currentBottom + slices + 1;
            const int nextTop = currentTop + 1;

            this->faces.push_back(utils::TriangleFace(currentBottom, currentTop, nextTop));
            this->faces.push_back(utils::TriangleFace(currentBottom, nextTop, nextBottom));
        }
    }

    const int topStackStart = topVertexIndex - slices - 1;
    for (int jSlice = 0; jSlice < slices; jSlice++) {
        const int current = topStackStart + jSlice;
        const int next = current + 1;
        this->faces.push_back(utils::TriangleFace(current, topVertexIndex, next));
    }
}

}
