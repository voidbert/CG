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

#define GLM_ENABLE_EXPERIMENTAL
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <glm/gtx/hash.hpp>
#include <optional>
#include <unordered_map>
#include <utility>

#include "utils/WavefrontOBJ.hpp"

namespace std {
template<>
struct hash<tuple<glm::vec4, glm::vec2, glm::vec3>> {
    size_t operator()(const tuple<glm::vec4, glm::vec2, glm::vec3> &t) const {
        const size_t h1 = hash<glm::vec4>()(get<0>(t));
        const size_t h2 = hash<glm::vec2>()(get<1>(t));
        const size_t h3 = hash<glm::vec3>()(get<2>(t));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
}

namespace utils {

WavefrontOBJ::WavefrontOBJ() {}

WavefrontOBJ::WavefrontOBJ(const std::string &filename) {
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open OBJ file: " + filename);
    }

    std::string line;
    std::optional<bool> completeFaces;
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

                if (completeFaces.value_or(false)) {
                    throw std::runtime_error("Can't mix face types in OBJ file: " + filename);
                }

                this->normals.push_back(glm::vec3(x, y, z));
                completeFaces = std::make_optional<bool>(false);
            } else if (match.str(10).length() > 0) {
                const int p1 = std::stoi(match.str(10)) - 1;
                const int p2 = std::stoi(match.str(11)) - 1;
                const int p3 = std::stoi(match.str(12)) - 1;

                if (!completeFaces.value_or(true)) {
                    throw std::runtime_error("Can't mix face types in OBJ file: " + filename);
                }

                this->faces.push_back(TriangleFace(p1, p2, p3));
                completeFaces = std::make_optional<bool>(true);
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
                        [this](const int32_t &positionIndex) {
                            return positionIndex >= static_cast<int32_t>(this->positions.size());
                        })) {
            throw std::runtime_error("Invalid position indices in OBJ file: " + filename);
        }

        if (std::any_of(face.textureCoordinates.cbegin(),
                        face.textureCoordinates.cend(),
                        [this](const int32_t &textureCoordinateIndex) {
                            return textureCoordinateIndex >=
                                static_cast<int32_t>(this->textureCoordinates.size());
                        })) {
            throw std::runtime_error("Invalid texture coordinate indices in OBJ file: " + filename);
        }

        if (std::any_of(face.normals.cbegin(),
                        face.normals.cend(),
                        [this](const int32_t &normalIndex) {
                            return normalIndex >= static_cast<int32_t>(this->normals.size());
                        })) {
            throw std::runtime_error("Invalid normal indices in OBJ file: " + filename);
        }
    }

    for (const TriangleFace &face : this->faces) {
        if (std::any_of(
                face.textureCoordinates.cbegin(),
                face.textureCoordinates.cend(),
                [](const int32_t &textureCoordinateIndex) { return textureCoordinateIndex < 0; })) {

            this->generateNormals();
            break;
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

std::tuple<std::vector<glm::vec4>,
           std::vector<glm::vec2>,
           std::vector<glm::vec4>,
           std::vector<uint32_t>>
    WavefrontOBJ::getIndexedVertices() const {

    std::unordered_map<std::tuple<glm::vec4, glm::vec2, glm::vec3>, int32_t> addedVertices;
    std::vector<glm::vec4> bufferPositions;
    std::vector<glm::vec2> bufferTextureCoordinates;
    std::vector<glm::vec4> bufferNormals;
    std::vector<uint32_t> indices;

    for (const TriangleFace &face : faces) {
        for (int i = 0; i < 3; ++i) {
            const glm::vec4 position = this->positions[face.positions[i]];

            glm::vec2 textureCoordinate = glm::vec2(0.0f, 0.0f);
            if (face.textureCoordinates[i] >= 0) {
                textureCoordinate = this->textureCoordinates[face.textureCoordinates[i]];
            }

            glm::vec3 normal;
            if (face.textureCoordinates[i] >= 0) {
                normal = this->normals[face.normals[i]];
            } else {
                normal = this->normals[face.positions[i]];
            }

            const std::tuple<glm::vec4, glm::vec2, glm::vec3> key =
                std::make_tuple(position, textureCoordinate, normal);
            auto it = addedVertices.find(key);

            uint32_t bufferIndex;
            if (it == addedVertices.end()) {
                bufferPositions.push_back(position);
                bufferTextureCoordinates.push_back(textureCoordinate);
                bufferNormals.push_back(glm::vec4(normal, 0.0f));

                bufferIndex = bufferPositions.size() - 1;
                addedVertices[key] = bufferIndex;
            } else {
                bufferIndex = it->second;
            }

            indices.push_back(bufferIndex);
        }
    }

    return std::make_tuple(bufferPositions, bufferTextureCoordinates, bufferNormals, indices);
}

void WavefrontOBJ::generateNormals() {
    std::unordered_map<glm::vec4, glm::vec3> averageNormals;

    for (const TriangleFace &face : this->faces) {
        // Get points and sides of the triangle
        const glm::vec4 p0 = this->positions[face.positions[0]];
        const glm::vec4 p1 = this->positions[face.positions[1]];
        const glm::vec4 p2 = this->positions[face.positions[2]];

        const glm::vec3 v0 = glm::vec3(p1) - glm::vec3(p0);
        const glm::vec3 v1 = glm::vec3(p2) - glm::vec3(p0);
        const glm::vec3 v2 = glm::vec3(p2) - glm::vec3(p1);

        // Calculate area of triangle via Heron's formula
        const float sideLength0 = glm::length(v0);
        const float sideLength1 = glm::length(v1);
        const float sideLength2 = glm::length(v2);
        const float semiPerimeter = 0.5f * (sideLength0 + sideLength1 + sideLength2);
        const float area = sqrtf(semiPerimeter * (semiPerimeter - sideLength0) *
                                 (semiPerimeter - sideLength1) * (semiPerimeter - sideLength2));

        // Calculate weighted average
        const glm::vec3 normal = glm::normalize(glm::cross(v0, v1)) * area;
        averageNormals[p0] += normal;
        averageNormals[p1] += normal;
        averageNormals[p2] += normal;
    }

    for (const glm::vec4 &position : this->positions) {
        const glm::vec3 perpendicular = averageNormals[position];
        this->normals.push_back(glm::normalize(perpendicular));
    }
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
