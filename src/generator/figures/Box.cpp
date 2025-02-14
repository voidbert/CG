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
#include <iostream>
#include <vector>

#include "generator/figures/Box.hpp"

Box::Box(float size, int divisions) {
    float step = size / divisions;
    float half = size / 2.0f;

    for (int face = 0; face < 6; face++) {
        glm::vec3 normal;
        int axis1, axis2, fixed;

        switch (face) {
            case 0:
                normal = { 0, 0, 1 };
                axis1 = 0;
                axis2 = 1;
                fixed = 2;
                break;
            case 1:
                normal = { 0, 0, -1 };
                axis1 = 0;
                axis2 = 1;
                fixed = 2;
                break;
            case 2:
                normal = { 0, 1, 0 };
                axis1 = 0;
                axis2 = 2;
                fixed = 1;
                break;
            case 3:
                normal = { 0, -1, 0 };
                axis1 = 0;
                axis2 = 2;
                fixed = 1;
                break;
            case 4:
                normal = { 1, 0, 0 };
                axis1 = 1;
                axis2 = 2;
                fixed = 0;
                break;
            case 5:
                normal = { -1, 0, 0 };
                axis1 = 1;
                axis2 = 2;
                fixed = 0;
                break;
        }

        for (int i = 0; i <= divisions; i++) {
            for (int j = 0; j <= divisions; j++) {
                glm::vec3 pos(0.0f);
                pos[axis1] = -half + j * step;
                pos[axis2] = -half + i * step;
                pos[fixed] = half * normal[fixed];

                vertices.push_back(Vertex(pos.x, pos.y, pos.z));
            }
        }

        int offset = face * (divisions + 1) * (divisions + 1);
        for (int i = 0; i < divisions; i++) {
            for (int j = 0; j < divisions; j++) {
                int idx = offset + i * (divisions + 1) + j;

                faces.push_back({ idx, idx + 1, idx + divisions + 1 });
                faces.push_back({ idx + 1, idx + divisions + 2, idx + divisions + 1 });
            }
        }
    }
}

void Box::toObj(const std::string &file3d) {
    std::ofstream file(file3d);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o ficheiro: " << file3d << "\n";
        return;
    }

    for (const auto &p : vertices) {
        file << "v " << p.position.x << " " << p.position.y << " " << p.position.z << "\n";
    }

    for (const auto &f : faces) {
        file << "f " << (f[0] + 1) << " " << (f[1] + 1) << " " << (f[2] + 1) << "\n";
    }

    file.close();
}
