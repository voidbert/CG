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
#include <glm/vec4.hpp>
#include <string>

#include "engine/render/ShaderProgram.hpp"

namespace engine::render {

class SolidColorShaderProgram : public ShaderProgram {
private:
    static const std::string vertexShaderSource, fragmentShaderSource;

public:
    SolidColorShaderProgram();
    SolidColorShaderProgram(const SolidColorShaderProgram &program) = delete;
    SolidColorShaderProgram(SolidColorShaderProgram &&program) = delete;

    void use() const;

    void setMatrix(const glm::mat4 &matrix) const;
    void setColor(const glm::vec4 &color) const;
};

}
