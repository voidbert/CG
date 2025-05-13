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
#include <glm/vec4.hpp>

#include "generator/figures/KleinBottle.hpp"

namespace generator::figures {

KleinBottle::KleinBottle(float radius, int slices, int stacks) {
    this->comment = "kleinBottle " + std::to_string(radius) + " " + std::to_string(slices) + " " +
        std::to_string(stacks);

    const float stackStep = glm::pi<float>() / stacks;
    const float sliceStep = glm::two_pi<float>() / slices;

    for (int iStack = 0; iStack <= stacks; iStack++) {
        const float theta = iStack * stackStep;

        for (int jSlice = 0; jSlice <= slices; jSlice++) {
            const float phi = jSlice * sliceStep;

            const float cosTheta = cosf(theta);
            const float sinTheta = sinf(theta);
            const float cosPhi = cosf(phi);
            const float sinPhi = sinf(phi);

            const float cosTheta2 = powf(cosTheta, 2);
            const float cosTheta3 = powf(cosTheta, 3);
            const float cosTheta4 = powf(cosTheta, 4);
            const float cosTheta5 = powf(cosTheta, 5);
            const float cosTheta6 = powf(cosTheta, 6);
            const float cosTheta7 = powf(cosTheta, 7);

            const float x = radius * (-2.0f / 15.0f) * cosTheta *
                (3 * cosPhi - 30 * sinTheta + 90 * cosTheta4 * sinTheta -
                 60 * cosTheta6 * sinTheta + 5 * cosTheta * cosPhi * sinTheta);

            const float y = radius * (-1.0f / 15.0f) * sinTheta *
                (3 * cosPhi - 3 * cosTheta2 * cosPhi - 48 * cosTheta4 * cosPhi +
                 48 * cosTheta6 * cosPhi - 60 * sinTheta + 5 * cosTheta * cosPhi * sinTheta -
                 5 * cosTheta3 * cosPhi * sinTheta - 80 * cosTheta5 * cosPhi * sinTheta +
                 80 * cosTheta7 * cosPhi * sinTheta);

            const float z = radius * (2.0f / 15.0f) * (3 + 5 * cosTheta * sinTheta) * sinPhi;

            positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    for (int iStack = 0; iStack < stacks; ++iStack) {
        for (int jSlice = 0; jSlice < slices; ++jSlice) {
            const int currentTop = iStack * (slices + 1) + jSlice;
            const int nextTop = currentTop + 1;
            const int currentBottom = currentTop + slices + 1;
            const int nextBottom = currentBottom + 1;

            faces.push_back(utils::TriangleFace(currentTop, nextBottom, currentBottom));
            faces.push_back(utils::TriangleFace(currentTop, nextTop, nextBottom));
        }
    }
}

}
