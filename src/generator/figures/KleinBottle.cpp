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
#include <vector>

#include "generator/figures/KleinBottle.hpp"

namespace generator::figures {

KleinBottle::KleinBottle(float radius, int slices, int stacks) {
    float stackStep = M_PI / stacks;
    float sliceStep = 2 * M_PI / slices;

    for (int iStack = 0; iStack < stacks; iStack++) {
        float theta = iStack * stackStep;

        for (int jSlice = 0; jSlice < slices; jSlice++) {
            float phi = jSlice * sliceStep;

            float cosTheta = cosf(theta);
            float sinTheta = sinf(theta);
            float cosPhi = cosf(phi);
            float sinPhi = sinf(phi);

            float cosTheta2 = pow(cosTheta, 2);
            float cosTheta3 = pow(cosTheta, 3);
            float cosTheta4 = pow(cosTheta, 4);
            float cosTheta5 = pow(cosTheta, 5);
            float cosTheta6 = pow(cosTheta, 6);
            float cosTheta7 = pow(cosTheta, 7);

            float x = radius * (-2.0f / 15.0f) * cosTheta *
                (3 * cosPhi - 30 * sinTheta + 90 * cosTheta4 * sinTheta -
                 60 * cosTheta6 * sinTheta + 5 * cosTheta * cosPhi * sinTheta);

            float y = radius * (-1.0f / 15.0f) * sinTheta *
                (3 * cosPhi - 3 * cosTheta2 * cosPhi - 48 * cosTheta4 * cosPhi +
                 48 * cosTheta6 * cosPhi - 60 * sinTheta + 5 * cosTheta * cosPhi * sinTheta -
                 5 * cosTheta3 * cosPhi * sinTheta - 80 * cosTheta5 * cosPhi * sinTheta +
                 80 * cosTheta7 * cosPhi * sinTheta);

            float z = radius * (2.0f / 15.0f) * (3 + 5 * cosTheta * sinTheta) * sinPhi;

            positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    for (int iStack = 0; iStack < stacks - 1; ++iStack) {
        for (int jSlice = 0; jSlice < slices; ++jSlice) {
            int currentTop = iStack * slices + jSlice;
            int nextTop = iStack * slices + (jSlice + 1) % slices;
            int currentBottom = (iStack + 1) * slices + jSlice;
            int nextBottom = (iStack + 1) * slices + (jSlice + 1) % slices;

            faces.push_back(utils::TriangleFace(currentTop, currentBottom, nextBottom));
            faces.push_back(utils::TriangleFace(currentTop, nextBottom, nextTop));
        }
    }
}

}
