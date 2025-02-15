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

#include <glm/vec3.hpp>

#include "generator/figures/Box.hpp"

namespace generator::figures {

Box::Box(float size, int divisions) {
    float step = size / divisions;
    float half = size / 2.0f;

    for (int face = 0; face < 6; face++) {
        glm::vec3 ivec, jvec;

        switch (face) {
            // Front and back
            case 0:
            case 1:
                ivec = glm::vec3(1.0f, 0.0f, 0.0f);
                jvec = glm::vec3(0.0f, 1.0f, 0.0f);
                break;
            // Right and left
            case 2:
            case 3:
                ivec = glm::vec3(0.0f, 1.0f, 0.0f);
                jvec = glm::vec3(0.0f, 0.0f, 1.0f);
                break;
            // Top and bottom
            case 4:
            case 5:
                ivec = glm::vec3(1.0f, 0.0f, 0.0f);
                jvec = glm::vec3(0.0f, 0.0f, 1.0f);
                break;
        }

        const bool positiveCoordinateFace = (face % 2) == 0;
        const float faceNormalMultiplier = positiveCoordinateFace ? 1.0f : -1.0f;

        const glm::vec3 faceNormal = glm::vec3(1.0f, 1.0f, 1.0f) - ivec - jvec;
        const glm::vec3 faceStartTranslate =
            (ivec + jvec + faceNormal * faceNormalMultiplier) * -half;

        for (int i = 0; i <= divisions; i++) {
            for (int j = 0; j <= divisions; j++) {
                const glm::vec3 vertexOffset = ivec * (i * step) + jvec * (j * step);
                this->positions.push_back(glm::vec4(faceStartTranslate + vertexOffset, 1.0f));
            }
        }

        int offset = face * (divisions + 1) * (divisions + 1);
        for (int i = 0; i < divisions; i++) {
            for (int j = 0; j < divisions; j++) {
                int idx = offset + i * (divisions + 1) + j;

                this->faces.push_back(utils::TriangleFace(idx, idx + 1, idx + divisions + 1));
                this->faces.push_back(
                    utils::TriangleFace(idx + 1, idx + divisions + 2, idx + divisions + 1));
            }
        }
    }
}

}
