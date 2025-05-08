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

#include "generator/figures/Plane.hpp"

namespace generator::figures {

Plane::Plane(float size, int divisions) {
    this->comment = "plane " + std::to_string(size) + " " + std::to_string(divisions);

    const float positionStep = size / divisions;
    const float positionStart = size / 2.0f;
    const float textureStep = 1.0f / divisions;

    this->normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

    for (int i = 0; i <= divisions; i++) {
        for (int j = 0; j <= divisions; j++) {
            this->positions.push_back(glm::vec4(i * positionStep - positionStart,
                                                0.0f,
                                                j * positionStep - positionStart,
                                                1.0));

            this->textureCoordinates.push_back(glm::vec2(i * textureStep, j * textureStep));
        }
    }

    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            const int currentBottom = i * (divisions + 1) + j;
            const int currentTop = currentBottom + divisions + 1;
            const int nextBottom = currentBottom + 1;
            const int nextTop = currentTop + 1;

            this->faces.push_back(utils::TriangleFace(currentBottom,
                                                      currentBottom,
                                                      0,
                                                      nextBottom,
                                                      nextBottom,
                                                      0,
                                                      currentTop,
                                                      currentTop,
                                                      0));
            this->faces.push_back(utils::TriangleFace(nextBottom,
                                                      nextBottom,
                                                      0,
                                                      nextTop,
                                                      nextTop,
                                                      0,
                                                      currentTop,
                                                      currentTop,
                                                      0));
        }
    }
}

}
