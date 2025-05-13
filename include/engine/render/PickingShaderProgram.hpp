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

#include <glm/glm.hpp>

#include "engine/render/ShaderProgram.hpp"

namespace engine::render {

class PickingShaderProgram : public ShaderProgram {
public:
    PickingShaderProgram();

    void setFullMatrix(const glm::mat4 &fullMatrix) const;
    void setColor(const glm::vec3 &color) const;

private:
    GLint fullMatrixUniformLocation;
    GLint colorUniformLocation;

    static const std::string vertexShaderSource;
    static const std::string fragmentShaderSource;
};

}
