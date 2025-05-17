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
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "generator/figures/Cylinder.hpp"

namespace generator::figures {

Cylinder::Cylinder(float radius, float height, int slices, int stacks, bool multiTextured) {
    const std::string multiTexturedString = multiTextured ? " multi-textured" : "";
    this->comment = "cylinder " + std::to_string(radius) + " " + std::to_string(height) + " " +
        std::to_string(slices) + " " + std::to_string(stacks) + multiTexturedString;

    const float sliceStep = glm::two_pi<float>() / slices;
    const float stackStep = height / stacks;

    this->positions.push_back(glm::vec4(0, 0, 0, 1.0f));
    this->normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    this->textureCoordinates.push_back(multiTextured ? glm::vec2(0.8125f, 0.1875f) :
                                                       glm::vec2(0.5f, 0.5f));

    for (int jSlice = 0; jSlice <= slices; jSlice++) {
        float angle = jSlice * sliceStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        this->positions.push_back(glm::vec4(x, 0, z, 1.0f));
        this->normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));

        float s = multiTextured ? (x / (2 * radius)) * 0.35f + 0.8125f : x / (2 * radius) + 0.5f;
        float t = multiTextured ? (z / (2 * radius)) * 0.35f + 0.1875f : z / (2 * radius) + 0.5f;
        this->textureCoordinates.push_back(glm::vec2(s, t));
    }

    for (int jSlice = 0; jSlice < slices; jSlice++) {
        this->faces.push_back(utils::TriangleFace(0,
                                                  0,
                                                  0,
                                                  jSlice + 1,
                                                  jSlice + 1,
                                                  jSlice + 1,
                                                  jSlice + 2,
                                                  jSlice + 2,
                                                  jSlice + 2));
    }

    int offset = this->positions.size();
    for (int iStack = 0; iStack <= stacks; iStack++) {
        float y = iStack * stackStep;

        float t = multiTextured ? 0.375f + (static_cast<float>(iStack) / stacks) * (1.0f - 0.375f) :
                                  static_cast<float>(iStack) / stacks;

        for (int jSlice = 0; jSlice <= slices; jSlice++) {
            float phi = jSlice * sliceStep;
            float x = radius * cos(phi);
            float z = radius * sin(phi);

            float s = 1.0f - static_cast<float>(jSlice) / slices;

            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
            this->normals.push_back(glm::normalize(glm::vec3(x, 0.0f, z)));
            this->textureCoordinates.push_back(glm::vec2(s, t));
        }
    }

    for (int iStack = 0; iStack < stacks; iStack++) {
        for (int jSlice = 0; jSlice < slices; jSlice++) {
            int currentBottom = offset + iStack * (slices + 1) + jSlice;
            int nextBottom = currentBottom + 1;
            int currentTop = currentBottom + slices + 1;
            int nextTop = currentTop + 1;

            this->faces.push_back(utils::TriangleFace(currentBottom,
                                                      currentBottom,
                                                      currentBottom,
                                                      currentTop,
                                                      currentTop,
                                                      currentTop,
                                                      nextTop,
                                                      nextTop,
                                                      nextTop));
            this->faces.push_back(utils::TriangleFace(currentBottom,
                                                      currentBottom,
                                                      currentBottom,
                                                      nextTop,
                                                      nextTop,
                                                      nextTop,
                                                      nextBottom,
                                                      nextBottom,
                                                      nextBottom));
        }
    }

    const int topCenterIndex = this->positions.size();
    this->positions.push_back(glm::vec4(0, height, 0, 1.0f));
    this->normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    this->textureCoordinates.push_back(multiTextured ? glm::vec2(0.4375f, 0.1875f) :
                                                       glm::vec2(0.5f, 0.5f));

    for (int jSlice = 0; jSlice <= slices; jSlice++) {
        float angle = jSlice * sliceStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        this->positions.push_back(glm::vec4(x, height, z, 1.0f));
        this->normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

        float s = multiTextured ? (x / (2 * radius)) * 0.35f + 0.4375f : x / (2 * radius) + 0.5f;
        float t = multiTextured ? (z / (2 * radius)) * 0.35f + 0.1875f : z / (2 * radius) + 0.5f;
        this->textureCoordinates.push_back(glm::vec2(s, t));
    }

    for (int jSlice = 0; jSlice < slices; jSlice++) {
        const int current = topCenterIndex + jSlice + 1;
        const int next = current + 1;
        this->faces.push_back(utils::TriangleFace(current,
                                                  current,
                                                  current,
                                                  topCenterIndex,
                                                  topCenterIndex,
                                                  topCenterIndex,
                                                  next,
                                                  next,
                                                  next));
    }
}

}
