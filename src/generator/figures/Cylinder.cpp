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

#include "generator/figures/Cylinder.hpp"

namespace generator::figures {
Cylinder::Cylinder(float radius, float height, int slices, int stacks) {
    float sliceStep = 2 * M_PI / slices;
    float stackStep = height / stacks;
    float halfHeight = height / 2.0f;

    for (int iStack = 0; iStack <= stacks; iStack++) {
        float y = iStack * stackStep;

        for (int jSlice = 0; jSlice < slices; jSlice++) {
            float phi = jSlice * sliceStep;
            float x = radius * cosf(phi);
            float z = radius * sinf(phi);

            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    for (int iStack = 0; iStack < stacks; iStack++) {
        int topStart = iStack * slices;
        int bottomStart = (iStack + 1) * slices;

        for (int jSlice = 0; jSlice < slices; jSlice++) {
            int nextSlice = (jSlice + 1) % slices;
            int topNext = topStart + jSlice;
            int bottomNext = bottomStart + nextSlice;

            this->faces.push_back(utils::TriangleFace(topNext, bottomStart + jSlice, bottomNext));
            this->faces.push_back(utils::TriangleFace(topNext, bottomNext, topStart + nextSlice));
        }
    }

    int topCenter = this->positions.size();
    this->positions.push_back(glm::vec4(0, height, 0, 1.0f));
    int bottomCenter = this->positions.size();
    this->positions.push_back(glm::vec4(0, 0, 0, 1.0f));

    int topBaseIndex = stacks * slices;
    int baseIndex = 0;

    for (int jSlice = 0; jSlice < slices; jSlice++) {
        int nextSlice = (jSlice + 1) % slices;

        this->faces.push_back(
            utils::TriangleFace(topCenter, topBaseIndex + nextSlice, topBaseIndex + jSlice));
        this->faces.push_back(
            utils::TriangleFace(bottomCenter, baseIndex + jSlice, baseIndex + nextSlice));
    }
}
}
