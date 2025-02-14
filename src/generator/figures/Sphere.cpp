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

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "generator/figures/Sphere.hpp"
#include "utils/WavefrontOBJ.hpp"

Sphere::Sphere(float radius, int slices, int stacks) {

    float stackStep = M_PI / stacks;
    float sliceStep = 2 * M_PI / slices;

    for (int iStack = 0; iStack <= stacks; iStack++) {
        float theta = iStack * stackStep;
        float y = radius * cos(theta);
        float xz = radius * sin(theta);

        for (int jSlice = 0; jSlice <= slices; jSlice++) {
            float phi = jSlice * sliceStep;
            float x = xz * sin(phi);
            float z = xz * cos(phi);

            vertices.push_back(Vertex(x, y, z));
        }
    }

    for (int iStack = 0; iStack < stacks; iStack++) {
        for (int jSlice = 0; jSlice < slices; jSlice++) {
            int curr = iStack * (slices + 1) + jSlice;
            int next = (iStack + 1) * (slices + 1) + jSlice;

            faces.push_back({ curr, next, next + 1 });
            faces.push_back({ curr, curr + 1, next + 1 });
        }
    }
}

void Sphere::toObj(const std::string &file3d) {
    writeObjFile(file3d, vertices, faces);
}
