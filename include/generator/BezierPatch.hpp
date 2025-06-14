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

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include "utils/WavefrontOBJ.hpp"

namespace generator {

class BezierPatch : public utils::WavefrontOBJ {

public:
    BezierPatch(const std::string &filename, int tessellation);

private:
    int stringToUnsignedInt(std::string str);
    float stringToFloat(std::string str);
    glm::vec3 stringToVector(const std::string &str);
    std::vector<int> stringToArray(const std::string &str);
    glm::mat4 preComputePatchMatrix(const std::vector<glm::vec3> &patchPoints, int coordinate);
    glm::vec3 evaluateBezierSurface(const glm::mat4 &mx,
                                    const glm::mat4 &my,
                                    const glm::mat4 &mz,
                                    float u,
                                    float v);
    glm::vec3 evaluateBezierSurfaceDu(const glm::mat4 &mx,
                                      const glm::mat4 &my,
                                      const glm::mat4 &mz,
                                      float u,
                                      float v);
    glm::vec3 evaluateBezierSurfaceDv(const glm::mat4 &mx,
                                      const glm::mat4 &my,
                                      const glm::mat4 &mz,
                                      float u,
                                      float v);
};

}
