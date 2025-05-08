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

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "engine/render/ShadedShaderProgram.hpp"

namespace engine::render {

ShadedShaderProgram::ShadedShaderProgram() :
    ShaderProgram(ShadedShaderProgram::vertexShaderSource,
                  ShadedShaderProgram::fragmentShaderSource) {}

void ShadedShaderProgram::use() const {
    ShaderProgram::use();
    this->setMatrix(glm::mat4(1.0f));
}

void ShadedShaderProgram::setMatrix(const glm::mat4 &matrix) const {
    glUniformMatrix4fv(0, 1, false, glm::value_ptr(matrix));
}

const std::string ShadedShaderProgram::vertexShaderSource = R"(
#version 460 core
layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inNormal;
layout (location = 2) in vec4 inTextureCoordinate;

layout (location = 0) uniform mat4 uniMatrix;

void main() {
    gl_Position = uniMatrix * inPosition;
}
)";

const std::string ShadedShaderProgram::fragmentShaderSource = R"(
#version 460 core
layout (location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)";

}
