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

#include "generator/figures/Torus.hpp"

namespace generator::figures {

Torus::Torus(float majorRadius, float minorRadius, int slices, int stacks) {
    float sliceStep = 2.0f * M_PI / slices;
    float stackStep = 2.0f * M_PI / stacks;

    for (int iStack = 0; iStack <= stacks; iStack++) {
        float v = iStack * stackStep;
        float cosV = cosf(v);
        float sinV = sinf(v);

        for (int jSlice = 0; jSlice <= slices; jSlice++) {
            float u = jSlice * sliceStep;
            float cosU = cosf(u);
            float sinU = sinf(u);

            float x = (majorRadius + minorRadius * cosV) * cosU;
            float y = minorRadius * sinV;
            float z = (majorRadius + minorRadius * cosV) * sinU;

            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    for (int iStack = 0; iStack < stacks; iStack++) {
        for (int jSlice = 0; jSlice < slices; jSlice++) {
            int curr = iStack * (slices + 1) + jSlice;
            int next = (iStack + 1) * (slices + 1) + jSlice;

            this->faces.push_back(utils::TriangleFace(curr, next, next + 1));
            this->faces.push_back(utils::TriangleFace(curr, curr + 1, next + 1));
        }
    }
}

}
