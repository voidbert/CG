// Copyright 2025 Ana Oliveira, Humberto Gomes, Mariana Rocha, Sara Lopes
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
#include <sstream>
#include <string>
#include <vector>

std::pair<std::vector<glm::vec4>, std::vector<std::vector<int>>>
    readObjFile(const std::string &filename) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(filename);

    std::vector<glm::vec4> vertices;
    std::vector<std::vector<int>> faces;

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            float x, y, z, w;
            ss >> x >> y >> z >> w;
            vertices.push_back(glm::vec4(x, y, z, w));
        } else if (type == "f") {
            std::vector<int> face;
            int index;
            while (ss >> index) {
                face.push_back(index - 1); // 1-based index to 0-based index
            }
            faces.push_back(face);
        }
    }
    file.close();
    return std::make_pair(vertices, faces);
}

void writeObjFile(const std::string &filename,
                  const std::vector<glm::vec4> &vertices,
                  const std::vector<std::vector<int>> &faces) {
    std::ofstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(filename, std::ios::out | std::ios::trunc);
    for (const auto &v : vertices) {
        file << "v " << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
    }

    for (const auto &f : faces) {
        file << "f";
        for (int idx : f) {
            file << " " << (idx + 1); // 0-based index to 1-based index
        }
        file << std::endl;
    }

    file.close();
}
