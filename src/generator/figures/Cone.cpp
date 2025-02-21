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

#include "generator/figures/Cone.hpp"

namespace generator::figures {

Cone::Cone(float radius, float height, int slices, int stacks) {

    float stackStep = height / stacks;
    float sliceStep = 2 * M_PI / slices;

    this->positions.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    for (int iStack = 0; iStack < stacks; iStack++) {
        float y = iStack * stackStep;
        float new_radius = ((height - y) * radius) / height;
        for (int jSlice = 0; jSlice < slices; jSlice++) {
            float angle = jSlice * sliceStep;
            float x = new_radius * cosf(angle);
            float z = new_radius * sinf(angle);
            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }
    this->positions.push_back(glm::vec4(0.0f, height, 0.0f, 1.0f));

    for (int jSlice = 0; jSlice < slices; jSlice++) {
        int nextSlice = (jSlice + 1) % slices;
        this->faces.push_back(utils::TriangleFace(0, 1 + jSlice, 1 + nextSlice));
    }

    int curr = 1;
    for (int iStack = 0; iStack < stacks - 1; iStack++) {
        for (int jSlice = 0; jSlice < slices; jSlice++) {
            int next = curr % slices + 1 + slices * iStack;
            int curr_top = curr + slices;
            int next_top = next + slices;
            this->faces.push_back(utils::TriangleFace(curr, curr_top, next_top));
            this->faces.push_back(utils::TriangleFace(curr, next_top, next));
            curr++;
        }
    }
    int top = slices * stacks + 1;
    for (int jSlice = 0; jSlice < slices; jSlice++) {
        int nextSlice = (curr + jSlice) % slices + slices * (stacks - 1);
        this->faces.push_back(utils::TriangleFace(curr + jSlice, top, nextSlice + 1));
    }
}

}
