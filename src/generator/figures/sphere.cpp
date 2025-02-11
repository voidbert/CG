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

#include "generator/figures/sphere.hpp"

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

            vertices.push_back(Point(x, y, z));
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
    // writeObjFile(file3d, std::make_pair(vertices, faces));
    std::ofstream file(file3d);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita: " << file3d << "\n";
        return;
    }

    int numTriangles = faces.size();
    int numVertices = vertices.size();

    file << numVertices << " " << numTriangles << "\n";

    for (const auto &p : vertices) {
        file << "v " << p.x << " " << p.y << " " << p.z << "\n";
    }

    for (const auto &f : faces) {
        file << "f " << (f[0] + 1) << " " << (f[1] + 1) << " " << (f[2] + 1) << "\n";
    }

    file.close();
}
