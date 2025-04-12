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

#include <cmath>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

#include "generator/BezierPatch.hpp"

namespace generator {

BezierPatch::BezierPatch(const std::string &filename, int tessellation) {
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open patch file: " + filename);
    }

    // Parse the patch file
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
                std::vector<int> indices = this->stringToArray(line);
                if (indices.size() != 16)
                    throw std::runtime_error("");

                patches.push_back(std::move(indices));
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

    // Generate the mesh
    for (const std::vector<int> &patch : patches) {
        // Gather the vertices
        std::vector<glm::vec3> patchPoints(patch.size());
        std::transform(patch.cbegin(), patch.cend(), patchPoints.begin(), [points](int idx) {
            return points[idx];
        });

        // Tessellate
        const glm::mat4 mx = this->preComputePatchMatrix(patchPoints, 0);
        const glm::mat4 my = this->preComputePatchMatrix(patchPoints, 1);
        const glm::mat4 mz = this->preComputePatchMatrix(patchPoints, 2);

        const float tessellationStep = 1.0f / tessellation;
        for (int i = 0; i < tessellation; ++i) {
            const glm::vec4 u = this->computeVectorPolynomial(i * tessellationStep);
            const glm::vec4 uNext = this->computeVectorPolynomial((i + 1) * tessellationStep);

            for (int j = 0; j < tessellation; ++j) {
                const glm::vec4 v = this->computeVectorPolynomial(j * tessellationStep);
                const glm::vec4 vNext = this->computeVectorPolynomial((j + 1) * tessellationStep);

                const glm::vec4 p1 = this->computePointCoordinates(mx, my, mz, u, v);
                const glm::vec4 p2 = this->computePointCoordinates(mx, my, mz, u, vNext);
                const glm::vec4 p3 = this->computePointCoordinates(mx, my, mz, uNext, v);
                const glm::vec4 p4 = this->computePointCoordinates(mx, my, mz, uNext, vNext);

                int i1 = this->positions.size();
                this->positions.push_back(p1);
                this->positions.push_back(p2);
                this->positions.push_back(p3);
                this->positions.push_back(p4);

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
    ret.z = this->stringToFloat(str.substr(c2 + 1));
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

glm::mat4 BezierPatch::preComputePatchMatrix(const std::vector<glm::vec3> &patchPoints,
                                             int coordinate) {

    const glm::mat4 m = {
        { -1, 3,  -3, 1 },
        { 3,  -6, 3,  0 },
        { -3, 3,  0,  0 },
        { 1,  0,  0,  0 }
    };
    const glm::mat4 transposed = glm::transpose(m);

    glm::mat4 pointsMatrix;
    float *coordinates = glm::value_ptr(pointsMatrix);
    for (int i = 0; i < 16; ++i) {
        coordinates[i] = patchPoints[i][coordinate];
    }

    return m * pointsMatrix * transposed;
}

glm::vec4 BezierPatch::computeVectorPolynomial(float uv) {
    return glm::vec4(powf(uv, 3), powf(uv, 2), uv, 1.0f);
}

glm::vec4 BezierPatch::computePointCoordinates(const glm::mat4 &my,
                                               const glm::mat4 &mx,
                                               const glm::mat4 &mz,
                                               const glm::vec4 &uVec,
                                               const glm::vec4 &vVec) {

    return glm::vec4(glm::dot(uVec * mx, vVec),
                     glm::dot(uVec * my, vVec),
                     glm::dot(uVec * mz, vVec),
                     1.0f);
}

}
