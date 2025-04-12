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

#include <iostream>
// #include <algorithm>
#include <fstream>
#include <glm/common.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stdexcept>

#include "generator/BezierPatch.hpp"

namespace generator {

glm::vec3 bezierCurve(const std::vector<glm::vec3> &points, float t) {
    glm::vec3 a = glm::mix(points[0], points[1], t);
    glm::vec3 b = glm::mix(points[1], points[2], t);
    glm::vec3 c = glm::mix(points[2], points[3], t);
    glm::vec3 d = glm::mix(a, b, t);
    glm::vec3 e = glm::mix(b, c, t);
    return glm::mix(d, e, t);
}

glm::vec3 bezierPatch(const std::vector<glm::vec3> &controlPoints, float u, float v) {
    std::vector<glm::vec3> uCurve(4);
    for (int i = 0; i < 4; ++i) {
        std::vector<glm::vec3> row = { controlPoints[i * 4 + 0],
                                       controlPoints[i * 4 + 1],
                                       controlPoints[i * 4 + 2],
                                       controlPoints[i * 4 + 3] };
        uCurve[i] = bezierCurve(row, u);
    }
    return bezierCurve(uCurve, v);
}

BezierPatch::BezierPatch(const std::string &filename, int tessellation) {
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open patch file: " + filename);
    }

    int numPatches = -1, numPoints = -1, remainingPatches = -1, remainingPoints = -1;
    std::vector<std::vector<int>> patches;
    std::vector<glm::vec3> points;

    std::string line;
    int lineNumber = 1;
    try {
        while (std::getline(file, line)) {
            // Trim the line first
            line.erase(line.find_last_not_of(" \t\n\r") + 1);
            line.erase(0, line.find_first_not_of(" \t\n\r"));

            if (numPatches < 0) {
                // Number of patches
                remainingPatches = numPatches = this->stringToUnsignedInt(line);
                patches.reserve(numPatches);
            } else if (remainingPatches > 0) {
                // Patch indices
                patches.push_back(this->stringToArray(line));
                remainingPatches--;
            } else if (numPoints < 0) {
                // Number of points
                remainingPoints = numPoints = this->stringToUnsignedInt(line);
                points.reserve(numPoints);
            } else if (remainingPoints > 0) {
                // Point coordinates
                points.push_back(this->stringToVector(line));
                remainingPoints--;
            } else if (line.length() != 0) {
                throw std::runtime_error("");
            }

            lineNumber++;
        }
    } catch (const std::exception &e) {
        throw std::runtime_error("Failed to parse patch file " + filename + ": line " +
                                 std::to_string(lineNumber));
    }

    for (const std::vector<int> &patch : patches) {
        // Gather the vertices
        std::vector<glm::vec3> patchPoints(patch.size());
        std::transform(patch.cbegin(), patch.cend(), points.begin(), [points](int idx) {
            return points[idx];
        });

        // Generate the mesh
        for (int i = 0; i < tessellation; ++i) {
            float u = static_cast<float>(i) / tessellation;
            float uNext = static_cast<float>(i + 1) / tessellation;

            for (int j = 0; j < tessellation; ++j) {
                float v = static_cast<float>(j) / tessellation;
                float vNext = static_cast<float>(j + 1) / tessellation;

                glm::vec3 p1 = bezierPatch(patchPoints, u, v);
                glm::vec3 p2 = bezierPatch(patchPoints, u, vNext);
                glm::vec3 p3 = bezierPatch(patchPoints, uNext, v);
                glm::vec3 p4 = bezierPatch(patchPoints, uNext, vNext);

                std::cout << glm::to_string(p1) << std::endl;

                int i1 = this->positions.size();
                this->positions.push_back(glm::vec4(p1, 1.0f));
                this->positions.push_back(glm::vec4(p2, 1.0f));
                this->positions.push_back(glm::vec4(p3, 1.0f));
                this->positions.push_back(glm::vec4(p4, 1.0f));

                this->faces.push_back(utils::TriangleFace(i1, i1 + 2, i1 + 1));
                this->faces.push_back(utils::TriangleFace(i1 + 1, i1 + 2, i1 + 3));
            }
        }
    }
}

int BezierPatch::stringToUnsignedInt(std::string str) {
    str.erase(str.find_last_not_of(" \t\n\r") + 1);
    str.erase(0, str.find_first_not_of(" \t\n\r"));

    size_t charactersParsed;
    const int ret = std::stoul(str, &charactersParsed);
    if (charactersParsed != str.length())
        throw std::invalid_argument("str is not an unsigned integer");

    return ret;
}

float BezierPatch::stringToFloat(std::string str) {
    str.erase(str.find_last_not_of(" \t\n\r") + 1);
    str.erase(0, str.find_first_not_of(" \t\n\r"));

    size_t charactersParsed;
    const float ret = std::stof(str, &charactersParsed);
    if (charactersParsed != str.length())
        throw std::invalid_argument("str is not a floating-point number");

    return ret;
}

glm::vec3 BezierPatch::stringToVector(const std::string &str) {
    const int c1 = str.find(',');
    const int c2 = str.find(',', c1 + 1);
    if (c1 == -1 || c2 == -1)
        throw std::invalid_argument("str is not an vector");

    glm::vec3 ret;
    ret.x = this->stringToFloat(str.substr(0, c1));
    ret.y = this->stringToFloat(str.substr(c1 + 1, c2 - c1 - 1));
    ret.y = this->stringToFloat(str.substr(c2 + 1));
    return ret;
}

std::vector<int> BezierPatch::stringToArray(const std::string &str) {
    std::vector<int> ret;

    int start = 0;
    do {
        const int comma = str.find(',', start);

        const std::string value = str.substr(start, comma < 0 ? str.length() : comma - start);
        ret.push_back(this->stringToUnsignedInt(value));

        start = comma + 1;
    } while (start != 0);

    return ret;
}

}
