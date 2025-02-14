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
#include <sstream>

#include "utils/WavefrontOBJ.hpp"

WavefrontOBJ::WavefrontOBJ() : positions(), faces() {}

WavefrontOBJ::WavefrontOBJ(const std::string &filename) : positions(), faces() {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(filename);

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            this->positions.push_back(glm::vec4(x, y, z, 1.0f));

            // TODO - support for w coordinate
        } else if (type == "f") {
            unsigned int p1, p2, p3;
            ss >> p1 >> p2 >> p3;

            // 1-based index to 0-based index
            this->faces.push_back(TriangleFace(p1 - 1, p2 - 1, p3 - 1));
        }

        // TODO - comment support
    }

    // TODO - error checking
}

void WavefrontOBJ::writeToFile(const std::string &filename) const {
    std::ofstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(filename, std::ios::out | std::ios::trunc);

    for (const glm::vec4 &v : this->positions) {
        file << "v " << v.x << " " << v.y << " " << v.z;
        if (v.w != 0.0f)
            file << " " << v.w;

        file << std::endl;
    }

    for (const TriangleFace &f : this->faces) {
        // 0-based index to 1-based index
        file << "f " << f.p1 + 1 << " " << f.p2 + 1 << " " << f.p3 + 1 << std::endl;
    }
}
