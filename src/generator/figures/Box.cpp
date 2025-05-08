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

Box::Box(float size, int divisions, bool multiTextured) {
    const std::string multiTexturedString = multiTextured ? " multi-textured" : "";
    this->comment =
        "box " + std::to_string(size) + " " + std::to_string(divisions) + multiTexturedString;

    const float positionStep = size / divisions;
    const float positionHalf = size / 2.0f;

    float textureStepX, textureStepY;
    if (multiTextured) {
        textureStepX = 1.0f / (4 * divisions);
        textureStepY = 1.0f / (3 * divisions);
    } else {
        textureStepX = textureStepY = 1.0f / divisions;
    }

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
                ivec = glm::vec3(0.0f, 0.0f, 1.0f);
                jvec = glm::vec3(1.0f, 0.0f, 0.0f);
                break;
        }

        const bool positiveCoordinateFace = (face % 2) == 0;
        const float faceNormalMultiplier = positiveCoordinateFace ? 1.0f : -1.0f;

        const glm::vec3 faceNormal = glm::vec3(1.0f, 1.0f, 1.0f) - ivec - jvec;
        this->normals.push_back(faceNormal * -faceNormalMultiplier);
        const int faceNormalIndex = this->normals.size() - 1;

        const glm::vec3 positionStart =
            (ivec + jvec + faceNormal * faceNormalMultiplier) * -positionHalf;

        glm::vec2 textureCoordinateStart;
        if (multiTextured) {
            switch (face) {
                case 0: // Front
                    textureCoordinateStart = glm::vec2(0.25f, 1.0f / 3.0f);
                    break;
                case 1: // Back
                    textureCoordinateStart = glm::vec2(0.75f, 1.0f / 3.0f);
                    break;
                case 2: // Right
                    textureCoordinateStart = glm::vec2(0.5f, 1.0f / 3.0f);
                    break;
                case 3: // Left
                    textureCoordinateStart = glm::vec2(0.0f, 1.0f / 3.0f);
                    break;
                case 4: // Top
                    textureCoordinateStart = glm::vec2(0.25f, 2.0f / 3.0f);
                    break;
                case 5: // Bottom
                    textureCoordinateStart = glm::vec2(0.25f, 0.0f);
                    break;
            }
        } else {
            textureCoordinateStart = glm::vec2(0.0f, 0.0f);
        }

        for (int i = 0; i <= divisions; i++) {
            for (int j = 0; j <= divisions; j++) {
                const glm::vec3 positionOffset =
                    ivec * (i * positionStep) + jvec * (j * positionStep);
                this->positions.push_back(glm::vec4(positionStart + positionOffset, 1.0f));

                if (multiTextured || face == 0) {
                    const glm::vec2 textureCoordinateOffset =
                        glm::vec2(textureStepX * i, textureStepY * j);

                    this->textureCoordinates.push_back(textureCoordinateStart +
                                                       textureCoordinateOffset);
                }
            }
        }

        const int faceOffset = face * (divisions + 1) * (divisions + 1);
        for (int i = 0; i < divisions; i++) {
            for (int j = 0; j < divisions; j++) {
                const int currentBottom = faceOffset + i * (divisions + 1) + j;
                const int currentTop = currentBottom + divisions + 1;
                const int nextBottom = currentBottom + 1;
                const int nextTop = currentTop + 1;

                const int textureModulus = multiTextured ? this->textureCoordinates.size() :
                                                           ((divisions + 1) * (divisions + 1));

                if (positiveCoordinateFace) {
                    this->faces.push_back(utils::TriangleFace(nextBottom,
                                                              nextBottom % textureModulus,
                                                              faceNormalIndex,
                                                              currentTop,
                                                              currentTop % textureModulus,
                                                              faceNormalIndex,
                                                              currentBottom,
                                                              currentBottom % textureModulus,
                                                              faceNormalIndex));
                    this->faces.push_back(utils::TriangleFace(nextBottom,
                                                              nextBottom % textureModulus,
                                                              faceNormalIndex,
                                                              nextTop,
                                                              nextTop % textureModulus,
                                                              faceNormalIndex,
                                                              currentTop,
                                                              currentTop % textureModulus,
                                                              faceNormalIndex));
                } else {
                    this->faces.push_back(utils::TriangleFace(nextBottom,
                                                              nextBottom % textureModulus,
                                                              faceNormalIndex,
                                                              currentBottom,
                                                              currentBottom % textureModulus,
                                                              faceNormalIndex,
                                                              currentTop,
                                                              currentTop % textureModulus,
                                                              faceNormalIndex));
                    this->faces.push_back(utils::TriangleFace(nextTop,
                                                              nextTop % textureModulus,
                                                              faceNormalIndex,
                                                              nextBottom,
                                                              nextBottom % textureModulus,
                                                              faceNormalIndex,
                                                              currentTop,
                                                              currentTop % textureModulus,
                                                              faceNormalIndex));
                }
            }
        }
    }
}

}
