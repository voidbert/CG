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

#include "generator/figures/MobiusStrip.hpp"

namespace generator::figures {

MobiusStrip::MobiusStrip(float radius, float width, int twists, int slices, int stacks) {
    this->comment = "mobiusStrip " + std::to_string(radius) + " " + std::to_string(width) + " " +
        std::to_string(twists) + " " + std::to_string(slices) + " " + std::to_string(stacks);

    const float sliceStep = glm::two_pi<float>() / slices;
    const float stackStep = glm::two_pi<float>() / stacks;
    const float halfWidth = width / 2.0f;

    twists *= 2;

    for (int iSlice = 0; iSlice <= slices; iSlice++) {
        float t = iSlice * sliceStep;
        float cosT = cosf(t);
        float sinT = sinf(t);

        const float twistAngle = twists * t / 2.0f;
        const float cosTwist = cosf(twistAngle);
        const float sinTwist = sinf(twistAngle);

        for (int jStack = 0; jStack <= stacks; jStack++) {
            const float v = jStack * stackStep - 1.0f;

            const float x = (radius + v * halfWidth * cosTwist) * cosT;
            const float z = (radius + v * halfWidth * cosTwist) * sinT;
            const float y = v * halfWidth * sinTwist;

            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    for (int iSlice = 0; iSlice < slices; iSlice++) {
        for (int jStack = 0; jStack < stacks; jStack++) {
            const int currentBottom = iSlice * (stacks + 1) + jStack;
            const int nextBottom = currentBottom + 1;
            const int currentTop = currentBottom + stacks + 1;
            const int nextTop = currentTop + 1;

            this->faces.push_back(utils::TriangleFace(currentBottom, currentTop, nextBottom));
            this->faces.push_back(utils::TriangleFace(currentTop, currentBottom, nextBottom));
            this->faces.push_back(utils::TriangleFace(nextBottom, currentTop, nextTop));
            this->faces.push_back(utils::TriangleFace(currentTop, nextBottom, nextTop));
        }
    }
}

}
