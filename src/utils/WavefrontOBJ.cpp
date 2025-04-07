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

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <unordered_map>

#include "utils/WavefrontOBJ.hpp"

namespace utils {

WavefrontOBJ::WavefrontOBJ() : positions(), faces() {}

WavefrontOBJ::WavefrontOBJ(const std::string &filename) : positions(), faces() {
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open OBJ file: " + filename);
    }

    std::string line;
    int lineNumber = 1;
    while (std::getline(file, line)) {
        std::smatch match;
        if (std::regex_match(line, match, WavefrontOBJ::lineRegex)) {
            if (match.str(1).length() > 0) {
                const float x = std::atof(match.str(1).c_str());
                const float y = std::atof(match.str(2).c_str());
                const float z = std::atof(match.str(3).c_str());

                std::string wString = match.str(4).c_str();
                float w = 1.0f;
                if (wString.length() > 0) {
                    w = std::atof(wString.c_str());
                }

                this->positions.push_back(glm::vec4(x, y, z, w));
            } else if (match.str(5).length() > 0) {
                const int p1 = std::atoi(match.str(5).c_str()) - 1;
                const int p2 = std::atoi(match.str(6).c_str()) - 1;
                const int p3 = std::atoi(match.str(7).c_str()) - 1;

                this->faces.push_back(TriangleFace(p1, p2, p3));
            }
        } else {
            throw std::runtime_error("Failed to parse OBJ file " + filename + ": line " +
                                     std::to_string(lineNumber));
        }

        lineNumber++;
    }

    if (file.bad()) {
        throw std::ios_base::failure("Error while reading from OBJ file: " + filename);
    }

    for (const TriangleFace &face : this->faces) {
        if (std::any_of(face.positions.cbegin(),
                        face.positions.cend(),
                        [this](const uint32_t &positionIndex) {
                            return positionIndex >= this->positions.size();
                        })) {
            throw std::runtime_error("Invalid indices in OBJ file: " + filename);
        }
    }
}

void WavefrontOBJ::writeToFile(const std::string &filename) const {
    std::ofstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(filename, std::ios::out | std::ios::trunc);

    for (const glm::vec4 &v : this->positions) {
        file << "v " << v.x << " " << v.y << " " << v.z;
        if (v.w != 1.0f)
            file << " " << v.w;

        file << std::endl;
    }

    for (const TriangleFace &f : this->faces) {
        // 0-based index to 1-based index
        file << "f ";
        file << f.positions[0] + 1 << " ";
        file << f.positions[1] + 1 << " ";
        file << f.positions[2] + 1 << std::endl;
    }
}

std::pair<std::vector<Vertex>, std::vector<uint32_t>> WavefrontOBJ::getIndexedVertices() const {
    std::unordered_map<Vertex, int32_t> addedVertices;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (const TriangleFace &face : faces) {
        for (int i = 0; i < 3; ++i) {
            Vertex vertex(this->positions[face.positions[i]]);

            auto it = addedVertices.find(vertex);
            uint32_t bufferIndex;

            if (it == addedVertices.end()) {
                vertices.push_back(vertex);
                bufferIndex = vertices.size() - 1;
                addedVertices[vertex] = bufferIndex;
            } else {
                bufferIndex = it->second;
            }

            indices.push_back(bufferIndex);
        }
    }

    return std::make_pair(vertices, indices);
}

// clang-format off
std::regex WavefrontOBJ::lineRegex(
    "\\s*"
    "|"
    "#.*"
    "|"
    "v\\s+" "((?:-|\\+)?[0-9]+(?:\\.[0-9]+)?(?:e(?:-|\\+)?[0-9]+)?)\\s+"
            "((?:-|\\+)?[0-9]+(?:\\.[0-9]+)?(?:e(?:-|\\+)?[0-9]+)?)\\s+"
            "((?:-|\\+)?[0-9]+(?:\\.[0-9]+)?(?:e(?:-|\\+)?[0-9]+)?)"
            "(\\s+(?:-|\\+)?[0-9]+(?:\\.[0-9]+)?(?:e[0-9]+)?)?\\s*"
    "|"
    "f\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s*", std::regex_constants::ECMAScript);
// clang-format on

}
