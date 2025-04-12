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

#include <algorithm>
#include <fstream>
#include <glm/glm.hpp>
#include <sstream>
#include <stdexcept>
#include <vector>

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

BezierPatch::BezierPatch(const std::string &filePath, int tessellation) {
    std::ifstream file(filePath);
    if (!file.is_open())
        throw std::runtime_error("Failed to open .patch file");

    int numPatches;
    file >> numPatches;

    std::vector<std::vector<int>> patches(numPatches);
    for (int i = 0; i < numPatches; ++i) {
        for (int j = 0; j < 16; ++j) {
            int index;
            file >> index;
            if (j < 15) {
                char c;
                file >> c; // read comma
            }
            patches[i].push_back(index);
        }
    }

    int numPoints;
    file >> numPoints;

    std::vector<glm::vec3> controlPoints(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        float x, y, z;
        char comma;
        file >> x >> comma >> y >> comma >> z;
        controlPoints[i] = glm::vec3(x, y, z);
    }

    for (const auto &patch : patches) {
        std::vector<glm::vec3> points(patch.size());

        std::transform(patch.begin(), patch.end(), points.begin(), [&](int idx) {
            return controlPoints[idx];
        });

        for (int i = 0; i < tessellation; ++i) {
            float u = static_cast<float>(i) / tessellation;
            float uNext = static_cast<float>(i + 1) / tessellation;

            for (int j = 0; j < tessellation; ++j) {
                float v = static_cast<float>(j) / tessellation;
                float vNext = static_cast<float>(j + 1) / tessellation;

                glm::vec3 p1 = bezierPatch(points, u, v);
                glm::vec3 p2 = bezierPatch(points, u, vNext);
                glm::vec3 p3 = bezierPatch(points, uNext, v);
                glm::vec3 p4 = bezierPatch(points, uNext, vNext);

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

}
