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

#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

#include "utils/TriangleFace.hpp"

namespace utils {

class WavefrontOBJ {
private:
    static std::regex lineRegex;

protected:
    std::string comment;
    std::vector<glm::vec4> positions;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<glm::vec3> normals;
    std::vector<TriangleFace> faces;

    WavefrontOBJ();

public:
    explicit WavefrontOBJ(const std::string &filename);

    void writeToFile(const std::string &filename) const;
    std::tuple<std::vector<glm::vec4>, // Positions
               std::vector<glm::vec2>, // Texture coordinates
               std::vector<glm::vec4>, // Normals (padded)
               std::vector<uint32_t>> // Indices
        getIndexedVertices() const;

    // TODO - Mariana, please make generateNormals private when Sphere.cpp doesn't need it anymore

protected:
    void generateNormals();
};

}
