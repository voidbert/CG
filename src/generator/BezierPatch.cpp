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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>
#include <stdexcept>

#include "generator/BezierPatch.hpp"

namespace generator {

BezierPatch::BezierPatch(const std::string &filename, int tessellation) {
    this->comment = "bezier " + filename + " " + std::to_string(tessellation);

    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open patch file: " + filename);
    }

    // Parse the patch file
    std::vector<std::vector<int>> patches;
    std::vector<glm::vec3> points;

    int lineNumber = 1;
    try {
        std::string line;
        int numPatches = -1, numPoints = -1, remainingPatches = -1, remainingPoints = -1;

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
        const int patchOffset = this->positions.size();

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
        for (int i = 0; i <= tessellation; ++i) {
            const float u = i * tessellationStep;

            for (int j = 0; j <= tessellation; ++j) {
                const float v = j * tessellationStep;

                glm::vec3 pos = evaluateBezierSurface(mx, my, mz, u, v);
                glm::vec3 du = evaluateBezierSurfaceDu(mx, my, mz, u, v);
                glm::vec3 dv = evaluateBezierSurfaceDv(mx, my, mz, u, v);

                glm::vec3 normal = glm::cross(du, dv);
                if (glm::length(normal) > 1e-6f) {
                    normal = glm::normalize(normal);
                } else {
                    normal = glm::vec3(0.0f, 1.0f, 0.0f);
                }

                this->positions.push_back(glm::vec4(pos, 1.0f));
                this->normals.push_back(normal);
                this->textureCoordinates.push_back(glm::vec2(u, v));
            }
        }

        // Indexing
        for (int i = 0; i < tessellation; ++i) {
            for (int j = 0; j < tessellation; ++j) {
                const int currentBottom = patchOffset + i * (tessellation + 1) + j;
                const int currentTop = currentBottom + tessellation + 1;
                const int nextBottom = currentBottom + 1;
                const int nextTop = currentTop + 1;

                this->faces.push_back(utils::TriangleFace(currentBottom,
                                                          currentBottom,
                                                          currentBottom,
                                                          nextBottom,
                                                          nextBottom,
                                                          nextBottom,
                                                          currentTop,
                                                          currentTop,
                                                          currentTop));

                this->faces.push_back(utils::TriangleFace(nextBottom,
                                                          nextBottom,
                                                          nextBottom,
                                                          nextTop,
                                                          nextTop,
                                                          nextTop,
                                                          currentTop,
                                                          currentTop,
                                                          currentTop));
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

glm::vec3 BezierPatch::evaluateBezierSurface(const glm::mat4 &mx,
                                             const glm::mat4 &my,
                                             const glm::mat4 &mz,
                                             float u,
                                             float v) {
    glm::vec4 uVec(powf(u, 3), powf(u, 2), u, 1.0f);
    glm::vec4 vVec(powf(v, 3), powf(v, 2), v, 1.0f);

    glm::vec3 result;
    result.x = glm::dot(mx * uVec, vVec);
    result.y = glm::dot(my * uVec, vVec);
    result.z = glm::dot(mz * uVec, vVec);

    return result;
}

glm::vec3 BezierPatch::evaluateBezierSurfaceDu(const glm::mat4 &mx,
                                               const glm::mat4 &my,
                                               const glm::mat4 &mz,
                                               float u,
                                               float v) {
    glm::vec4 duVec(3 * powf(u, 2), 2 * u, 1.0f, 0.0f);
    glm::vec4 vVec(powf(v, 3), powf(v, 2), v, 1.0f);

    glm::vec3 result;
    result.x = glm::dot(mx * duVec, vVec);
    result.y = glm::dot(my * duVec, vVec);
    result.z = glm::dot(mz * duVec, vVec);

    return result;
}

glm::vec3 BezierPatch::evaluateBezierSurfaceDv(const glm::mat4 &mx,
                                               const glm::mat4 &my,
                                               const glm::mat4 &mz,
                                               float u,
                                               float v) {
    glm::vec4 uVec(powf(u, 3), powf(u, 2), u, 1.0f);
    glm::vec4 dvVec(3 * powf(v, 2), 2 * v, 1.0f, 0.0f);

    glm::vec3 result;
    result.x = glm::dot(mx * uVec, dvVec);
    result.y = glm::dot(my * uVec, dvVec);
    result.z = glm::dot(mz * uVec, dvVec);

    return result;
}

}
