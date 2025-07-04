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

#include "generator/figures/Sphere.hpp"

namespace generator::figures {

Sphere::Sphere(float radius, int slices, int stacks) {
    this->comment = "sphere " + std::to_string(radius) + " " + std::to_string(slices) + " " +
        std::to_string(stacks);

    const float stackStep = glm::pi<float>() / stacks;
    const float sliceStep = glm::two_pi<float>() / slices;
    const float textureSliceStep = 1.0f / slices;

    // Generate North pole
    this->positions.push_back(glm::vec4(0.0f, radius, 0.0f, 1.0f));
    this->normals.push_back(glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
    for (int jSlice = 0; jSlice < slices; jSlice++) {
        const float s = jSlice * textureSliceStep + textureSliceStep / 2;
        this->textureCoordinates.push_back(glm::vec2(s, 1.0f));
    }

    // Generate center
    for (int iStack = 1; iStack < stacks; iStack++) {
        const float theta = iStack * stackStep;
        const float y = radius * cosf(theta);
        const float xz = radius * sinf(theta);
        const float t = 1.0f - static_cast<float>(iStack) / stacks;

        for (int jSlice = 0; jSlice <= slices; jSlice++) {
            const float phi = jSlice * sliceStep;
            const float x = xz * sinf(phi);
            const float z = xz * cosf(phi);
            const float s = static_cast<float>(jSlice) / slices;

            this->positions.push_back(glm::vec4(x, y, z, 1.0f));
            this->normals.push_back(glm::normalize(glm::vec3(x, y, z)));
            this->textureCoordinates.push_back(glm::vec2(s, t));
        }
    }

    // Generate South pole
    const int southPoleIndex = this->positions.size();
    this->positions.push_back(glm::vec4(0.0f, -radius, 0.0f, 1.0f));
    this->normals.push_back(glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)));

    for (int jSlice = 0; jSlice < slices; jSlice++) {
        const float s = jSlice * textureSliceStep + textureSliceStep / 2;
        this->textureCoordinates.push_back(glm::vec2(s, 0.0f));
    }

    for (int jSlice = 0; jSlice < slices; jSlice++) {
        this->faces.push_back(utils::TriangleFace(0,
                                                  jSlice,
                                                  0,
                                                  jSlice + 1,
                                                  jSlice + slices,
                                                  jSlice + 1,
                                                  jSlice + 2,
                                                  jSlice + slices + 1,
                                                  jSlice + 2));
    }

    for (int iStack = 0; iStack < stacks - 2; iStack++) {
        for (int jSlice = 0; jSlice < slices; jSlice++) {
            const int currentTop = 1 + iStack * (slices + 1) + jSlice;
            const int nextTop = currentTop + 1;
            const int currentBottom = currentTop + (slices + 1);
            const int nextBottom = currentBottom + 1;

            this->faces.push_back(utils::TriangleFace(currentTop,
                                                      currentTop + slices - 1,
                                                      currentTop,
                                                      currentBottom,
                                                      currentBottom + slices - 1,
                                                      currentBottom,
                                                      nextBottom,
                                                      nextBottom + slices - 1,
                                                      nextBottom));
            this->faces.push_back(utils::TriangleFace(currentTop,
                                                      currentTop + slices - 1,
                                                      currentTop,
                                                      nextBottom,
                                                      nextBottom + slices - 1,
                                                      nextBottom,
                                                      nextTop,
                                                      nextTop + slices - 1,
                                                      nextTop));
        }
    }

    const int bottomStackStart = southPoleIndex - slices - 1;
    for (int jSlice = 0; jSlice < slices; jSlice++) {
        const int current = bottomStackStart + jSlice;
        const int next = current + 1;

        this->faces.push_back(utils::TriangleFace(southPoleIndex,
                                                  southPoleIndex + slices,
                                                  southPoleIndex,
                                                  next,
                                                  next + slices,
                                                  next,
                                                  current,
                                                  current + slices,
                                                  current));
    }
}

}
