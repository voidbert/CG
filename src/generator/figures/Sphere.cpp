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

#include "generator/figures/Sphere.hpp"

namespace generator::figures {

Sphere::Sphere(float radius, int slices, int stacks) {
    float stackStep = M_PI / stacks;
    float sliceStep = 2 * M_PI / slices;

    this->positions.push_back(glm::vec4(0.0f, radius, 0.0f, 1.0f));
    for (int iStack = 1; iStack < stacks; iStack++) {
        float theta = iStack * stackStep;
        float y = radius * cosf(theta);
        float xz = radius * sinf(theta);

        for (int jSlice = 0; jSlice < slices; jSlice++) {
            float phi = jSlice * sliceStep;
            float x = xz * sinf(phi);
            float z = xz * cosf(phi);

            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    int southPoleIndex = this->positions.size();
    this->positions.push_back(glm::vec4(0.0f, -radius, 0.0f, 1.0f));

    for (int jSlice = 0; jSlice < slices; jSlice++) {
        int nextSlice = (jSlice + 1) % slices + 1;
        this->faces.push_back(utils::TriangleFace(0, jSlice + 1, nextSlice));
    }

    for (int iStack = 0; iStack < stacks - 2; ++iStack) {
        int topStart = 1 + iStack * slices;
        int bottomStart = 1 + (iStack + 1) * slices;

        for (int jSlice = 0; jSlice < slices; ++jSlice) {
            int currentTop = topStart + jSlice;
            int nextTop = topStart + (jSlice + 1) % slices;
            int currentBottom = bottomStart + jSlice;
            int nextBottom = bottomStart + (jSlice + 1) % slices;

            faces.push_back(utils::TriangleFace(currentTop, currentBottom, nextBottom));
            faces.push_back(utils::TriangleFace(currentTop, nextBottom, nextTop));
        }
    }

    int bottom = 1 + (stacks - 2) * slices;
    for (int jSlice = 0; jSlice < slices; jSlice++) {
        int current = bottom + jSlice;
        int next = bottom + (jSlice + 1) % slices;
        this->faces.push_back(utils::TriangleFace(southPoleIndex, next, current));
    }
}

}
