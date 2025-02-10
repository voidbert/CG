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

#include <glm/glm.hpp>
#include <math.h>
#include <string>
#include <vector>

#include "generator/figures/sphere.hpp"

Sphere::Sphere(float radius, int slices, int stacks) {
    float stackAngle = M_PI / stacks;
    float sliceAngle = 2 * M_PI / slices;

    for (int iStack = 0; iStack < stacks; iStack++) {
        float phi = iStack * stackAngle;
        float y = radius * cos(phi);
        float xz = radius * sin(phi);

        for (int jSlice = 0; jSlice < slices; jSlice++) {
            float theta = jSlice * sliceAngle;
            float x = xz * cos(theta);
            float z = xz * sin(theta);
            vertices.push_back(glm::vec4(x, y, z, 1.0));

            int firstVertex = iStack * (slices + 1) + jSlice;
            int secondVertex = firstVertex + slices + 1;

            std::vector<int> triangle1 = { firstVertex, secondVertex, firstVertex + 1 };
            std::vector<int> triangle2 = { secondVertex, secondVertex + 1, firstVertex + 1 };
            faces.push_back(triangle1);
            faces.push_back(triangle2);
        }
    }
}

void Sphere::toObj(const std::string &file3d) {
    // writeObjFile(file3d, std::make_pair(vertices, faces));
}
