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

#include "generator/figures/Gear.hpp"

namespace generator::figures {
Gear::Gear(float majorRadius,
           float minorRadius,
           float height,
           int stacks,
           int teeth,
           float toothHeight) {

    int slices = 4 * teeth;
    float sliceStep = 2 * M_PI / slices;
    float stackStep = height / stacks;

    // Outer Cylindrical base
    for (int iStack = 0; iStack < stacks; iStack++) {
        float y = iStack * stackStep;

        for (int jSlice = 0; jSlice < slices; jSlice++) {
            float phi = jSlice * sliceStep;
            float x = majorRadius * cosf(phi);
            float z = majorRadius * sinf(phi);

            // If it is a tooth position, increase the majorRadius
            if (jSlice % (slices / teeth) < (slices / (2 * teeth))) {
                x *= 1.0f + toothHeight / majorRadius;
                z *= 1.0f + toothHeight / majorRadius;
            }

            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    // Inner cylindrical base
    for (int iStack = 0; iStack < stacks; iStack++) {
        float y = iStack * stackStep;

        for (int jSlice = 0; jSlice < slices; jSlice++) {
            float phi = jSlice * sliceStep;
            float x = minorRadius * cosf(phi);
            float z = minorRadius * sinf(phi);

            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    // Side faces
    for (int iStack = 0; iStack < stacks - 1; iStack++) {
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

    // Inner side faces
    int minorOffset = stacks * slices;
    for (int iStack = 0; iStack < stacks - 1; iStack++) {
        int topStart = minorOffset + iStack * slices;
        int bottomStart = minorOffset + (iStack + 1) * slices;

        for (int jSlice = 0; jSlice < slices; jSlice++) {
            int nextSlice = (jSlice + 1) % slices;
            int topNext = topStart + jSlice;
            int bottomNext = bottomStart + nextSlice;

            this->faces.push_back(utils::TriangleFace(topNext, bottomStart + jSlice, bottomNext));
            this->faces.push_back(utils::TriangleFace(topNext, bottomNext, topStart + nextSlice));
        }
    }

    // Top and bottom
    int topBaseIndex = (stacks - 1) * slices;
    int minorTopBaseIndex = minorOffset + (stacks - 1) * slices;
    int baseIndex = 0;
    int minorBaseIndex = minorOffset;

    for (int jSlice = 0; jSlice < slices; jSlice++) {
        int nextSlice = (jSlice + 1) % slices;

        // Top
        this->faces.push_back(utils::TriangleFace(topBaseIndex + jSlice,
                                                  minorTopBaseIndex + jSlice,
                                                  minorTopBaseIndex + nextSlice));
        this->faces.push_back(utils::TriangleFace(topBaseIndex + jSlice,
                                                  minorTopBaseIndex + nextSlice,
                                                  topBaseIndex + nextSlice));

        // Bottom
        this->faces.push_back(utils::TriangleFace(baseIndex + jSlice,
                                                  minorBaseIndex + nextSlice,
                                                  minorBaseIndex + jSlice));
        this->faces.push_back(utils::TriangleFace(baseIndex + jSlice,
                                                  baseIndex + nextSlice,
                                                  minorBaseIndex + nextSlice));
    }
}
}
