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

#include "generator/figures/Gear.hpp"

namespace generator::figures {

Gear::Gear(float majorRadius,
           float minorRadius,
           float toothHeight,
           float height,
           int teeth,
           int stacks) {

    this->comment = "gear " + std::to_string(majorRadius) + " " + std::to_string(minorRadius) +
        " " + std::to_string(toothHeight) + " " + std::to_string(height) + " " +
        std::to_string(teeth) + " " + std::to_string(stacks);

    const int slices = 4 * teeth;
    const float sliceStep = glm::two_pi<float>() / slices;
    const float stackStep = height / stacks;

    // Outer cylindrical base
    for (int iStack = 0; iStack <= stacks; iStack++) {
        const float y = iStack * stackStep;

        for (int jSlice = 0; jSlice <= slices; jSlice++) {
            const float phi = jSlice * sliceStep;

            // Adjust radius for teeth height
            float radius = majorRadius;
            if (jSlice % 4 == 0 || jSlice % 4 == 1) {
                radius += toothHeight;
            }

            const float x = radius * cosf(phi);
            const float z = radius * sinf(phi);
            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    // Inner cylindrical base
    for (int iStack = 0; iStack <= stacks; iStack++) {
        const float y = iStack * stackStep;

        for (int jSlice = 0; jSlice <= slices; jSlice++) {
            const float phi = jSlice * sliceStep;
            const float x = minorRadius * cosf(phi);
            const float z = minorRadius * sinf(phi);

            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    // Side faces
    for (int iStack = 0; iStack < stacks; iStack++) {
        for (int jSlice = 0; jSlice < slices; jSlice++) {
            const int currentBottom = iStack * (slices + 1) + jSlice;
            const int nextBottom = currentBottom + 1;
            const int currentTop = currentBottom + slices + 1;
            const int nextTop = currentTop + 1;

            this->faces.push_back(utils::TriangleFace(nextTop, nextBottom, currentBottom));
            this->faces.push_back(utils::TriangleFace(nextTop, currentBottom, currentTop));
        }
    }

    // Inner side faces
    const int innerOffset = (stacks + 1) * (slices + 1);
    for (int iStack = 0; iStack < stacks; iStack++) {
        for (int jSlice = 0; jSlice < slices; jSlice++) {
            const int currentBottom = innerOffset + iStack * (slices + 1) + jSlice;
            const int nextBottom = currentBottom + 1;
            const int currentTop = currentBottom + slices + 1;
            const int nextTop = currentTop + 1;

            this->faces.push_back(utils::TriangleFace(nextTop, currentBottom, nextBottom));
            this->faces.push_back(utils::TriangleFace(nextTop, currentTop, currentBottom));
        }
    }

    // Top and bottom faces
    for (int jSlice = 0; jSlice < slices; jSlice++) {
        // Bottom
        const int bottomCurrentOuter = jSlice;
        const int bottomNextOuter = bottomCurrentOuter + 1;
        const int bottomCurrentInner = innerOffset + bottomCurrentOuter;
        const int bottomNextInner = bottomCurrentInner + 1;

        this->faces.push_back(
            utils::TriangleFace(bottomCurrentOuter, bottomNextInner, bottomCurrentInner));
        this->faces.push_back(
            utils::TriangleFace(bottomCurrentOuter, bottomNextOuter, bottomNextInner));

        // Top
        const int topCurrentOuter = jSlice + stacks * (slices + 1);
        const int topNextOuter = topCurrentOuter + 1;
        const int topCurrentInner = innerOffset + topCurrentOuter;
        const int topNextInner = topCurrentInner + 1;

        this->faces.push_back(utils::TriangleFace(topCurrentOuter, topCurrentInner, topNextInner));
        this->faces.push_back(utils::TriangleFace(topCurrentOuter, topNextInner, topNextOuter));
    }
}

}
