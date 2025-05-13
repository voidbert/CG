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

#include <glm/gtc/type_ptr.hpp>

#include "engine/render/PickingShaderProgram.hpp"

namespace engine::render {

PickingShaderProgram::PickingShaderProgram() :
    ShaderProgram(PickingShaderProgram::vertexShaderSource,
                  PickingShaderProgram::fragmentShaderSource),
    fullMatrixUniformLocation(this->getUniformLocation("uniFullMatrix")),
    colorUniformLocation(this->getUniformLocation("uniColor")) {}

void PickingShaderProgram::setFullMatrix(const glm::mat4 &fullMatrix) const {
    glUniformMatrix4fv(this->fullMatrixUniformLocation, 1, false, glm::value_ptr(fullMatrix));
}

void PickingShaderProgram::setColor(const glm::vec3 &color) const {
    glUniform3f(this->colorUniformLocation, color.r, color.g, color.b);
}

const std::string PickingShaderProgram::vertexShaderSource = R"(
#version 460 core
layout (location = 0) in vec4 inPosition; // Local space

uniform mat4 uniFullMatrix; // PVM

void main() {
    gl_Position = uniFullMatrix * inPosition; // Clip space
}
)";

const std::string PickingShaderProgram::fragmentShaderSource = R"(
#version 460 core
layout (location = 0) out vec4 outColor;

uniform vec3 uniColor;

void main() {
    outColor = vec4(uniColor, 1.0);
}
)";

}
