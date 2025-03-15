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

#include "generator/figures/MobiusStrip.hpp"

namespace generator::figures {

MobiusStrip::MobiusStrip(float radius, float width, int twist, int slices, int stacks) {

    for (int i = 0; i <= slices; i++) {
        float t = (float) i / slices * 2.0f * M_PI;
        float cosT = cos(t);
        float sinT = sin(t);
        float halfWidth = width / 2.0f;

        for (int j = 0; j <= stacks; j++) {
            float v = (float) j / stacks * 2.0f - 1.0f;
            float twistAngle = twist * t / 2.0f;
            float cosTwist = cos(twistAngle);
            float sinTwist = sin(twistAngle);

            float x = (radius + v * halfWidth * cosTwist) * cosT;
            float z = (radius + v * halfWidth * cosTwist) * sinT;
            float y = v * halfWidth * sinTwist;

            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
        }
    }

    for (int i = 0; i < slices - 1; i++) {
        for (int j = 0; j < stacks; j++) {
            int idx1 = i * (stacks + 1) + j;
            int idx2 = idx1 + stacks + 1;
            int idx3 = idx1 + 1;
            int idx4 = idx2 + 1;

            this->faces.push_back(utils::TriangleFace(idx1, idx2, idx3));
            this->faces.push_back(utils::TriangleFace(idx2, idx1, idx3));
            this->faces.push_back(utils::TriangleFace(idx3, idx2, idx4));
            this->faces.push_back(utils::TriangleFace(idx2, idx3, idx4));
        }
    }
    for (int j = 0; j < stacks; j++) {
        int idx1 = (slices - 1) * (stacks + 1) + j;
        int idx2;
        int idx3 = idx1 + 1;
        int idx4;
        if (twist % 2 == 0) {
            idx2 = j;
            idx4 = j + 1;
        } else {
            idx2 = stacks - j;
            idx4 = stacks - j - 1;
        }

        this->faces.push_back(utils::TriangleFace(idx1, idx2, idx3));
        this->faces.push_back(utils::TriangleFace(idx2, idx1, idx3));
        this->faces.push_back(utils::TriangleFace(idx3, idx2, idx4));
        this->faces.push_back(utils::TriangleFace(idx2, idx3, idx4));
    }
}

}
