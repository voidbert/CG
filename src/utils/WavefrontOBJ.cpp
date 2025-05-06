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

#include "utils/Vertex.hpp"
#include "utils/WavefrontOBJ.hpp"

namespace utils {

WavefrontOBJ::WavefrontOBJ() {}

WavefrontOBJ::WavefrontOBJ(const std::string &filename) {
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
                const float x = std::stof(match.str(1));
                const float y = std::stof(match.str(2));
                const float z = std::stof(match.str(3));

                std::string wString = match.str(4);
                float w = 1.0f;
                if (wString.length() > 0) {
                    w = std::stof(wString);
                }

                this->positions.push_back(glm::vec4(x, y, z, w));
            } else if (match.str(5).length() > 0) {
                const float u = std::stof(match.str(5));
                const float v = std::stof(match.str(6));

                this->textureCoordinates.push_back(glm::vec2(u, v));
            } else if (match.str(7).length() > 0) {
                const float x = std::stof(match.str(7));
                const float y = std::stof(match.str(8));
                const float z = std::stof(match.str(9));

                this->normals.push_back(glm::vec3(x, y, z));
            } else if (match.str(10).length() > 0) {
                const int p1 = std::stoi(match.str(10)) - 1;
                const int p2 = std::stoi(match.str(11)) - 1;
                const int p3 = std::stoi(match.str(12)) - 1;

                this->faces.push_back(TriangleFace(p1, p2, p3));
            } else if (match.str(13).length() > 0) {
                const int p1 = std::stoi(match.str(13)) - 1;
                const int t1 = std::stoi(match.str(14)) - 1;
                const int n1 = std::stoi(match.str(15)) - 1;

                const int p2 = std::stoi(match.str(16)) - 1;
                const int t2 = std::stoi(match.str(17)) - 1;
                const int n2 = std::stoi(match.str(18)) - 1;

                const int p3 = std::stoi(match.str(19)) - 1;
                const int t3 = std::stoi(match.str(20)) - 1;
                const int n3 = std::stoi(match.str(21)) - 1;

                this->faces.push_back(TriangleFace(p1, t1, n1, p2, t2, n2, p3, t3, n3));
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

    if (this->comment.length() > 0)
        file << "# " << this->comment << std::endl;

    for (const glm::vec4 &v : this->positions) {
        file << "v " << v.x << " " << v.y << " " << v.z;
        if (v.w != 1.0f)
            file << " " << v.w;

        file << std::endl;
    }

    for (const glm::vec2 &v : this->textureCoordinates) {
        file << "vt " << v.x << " " << v.y << std::endl;
    }

    for (const glm::vec3 &v : this->normals) {
        file << "vn " << v.x << " " << v.y << " " << v.z << std::endl;
    }

    for (const TriangleFace &f : this->faces) {
        // 0-based index to 1-based index
        if (f.textureCoordinates[0] == -1) {
            file << "f ";
            file << f.positions[0] + 1 << " ";
            file << f.positions[1] + 1 << " ";
            file << f.positions[2] + 1 << std::endl;
        } else {
            file << "f ";

            file << f.positions[0] + 1 << "/";
            file << f.textureCoordinates[0] + 1 << "/";
            file << f.normals[0] + 1 << " ";

            file << f.positions[1] + 1 << "/";
            file << f.textureCoordinates[1] + 1 << "/";
            file << f.normals[1] + 1 << " ";

            file << f.positions[2] + 1 << "/";
            file << f.textureCoordinates[2] + 1 << "/";
            file << f.normals[2] + 1 << std::endl;
        }
    }
}

std::pair<std::vector<glm::vec4>, std::vector<uint32_t>> WavefrontOBJ::getIndexedVertices() const {
    std::unordered_map<Vertex, int32_t> addedVertices;
    std::vector<glm::vec4> bufferPositions;
    std::vector<uint32_t> indices;

    for (const TriangleFace &face : faces) {
        for (int i = 0; i < 3; ++i) {
            Vertex vertex(this->positions[face.positions[i]]);

            auto it = addedVertices.find(vertex);
            uint32_t bufferIndex;

            if (it == addedVertices.end()) {
                bufferPositions.push_back(vertex.position);
                bufferIndex = bufferPositions.size() - 1;
                addedVertices[vertex] = bufferIndex;
            } else {
                bufferIndex = it->second;
            }

            indices.push_back(bufferIndex);
        }
    }

    return std::make_pair(bufferPositions, indices);
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
    "vt\\s+" "((?:-|\\+)?[0-9]+(?:\\.[0-9]+)?(?:e(?:-|\\+)?[0-9]+)?)\\s+"
             "((?:-|\\+)?[0-9]+(?:\\.[0-9]+)?(?:e(?:-|\\+)?[0-9]+)?)\\s*"
    "|"
    "vn\\s+" "((?:-|\\+)?[0-9]+(?:\\.[0-9]+)?(?:e(?:-|\\+)?[0-9]+)?)\\s+"
             "((?:-|\\+)?[0-9]+(?:\\.[0-9]+)?(?:e(?:-|\\+)?[0-9]+)?)\\s+"
             "((?:-|\\+)?[0-9]+(?:\\.[0-9]+)?(?:e(?:-|\\+)?[0-9]+)?)\\s*"
    "|"
    "f\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s*"
    "|"
    "f\\s+([0-9]+)/([0-9]+)/([0-9]+)\\s+([0-9]+)/([0-9]+)/([0-9]+)\\s+([0-9]+)/([0-9]+)/([0-9]+)\\s*",
    std::regex_constants::ECMAScript);
// clang-format on

}
