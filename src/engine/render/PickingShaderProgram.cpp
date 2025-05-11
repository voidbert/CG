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

#include "engine/render/PickingShaderProgram.hpp"

namespace engine::render {

static const std::string vertexSource = R"(
#version 330 core
layout(location = 0) in vec3 position;
uniform mat4 modelViewProjectionMatrix;
uniform vec3 colorID;

flat out vec3 vertexColorID;

void main() {
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
    vertexColorID = colorID;
}
)";

static const std::string fragmentSource = R"(
    #version 330 core
    flat in vec3 vertexColorID;
    out vec4 fragColor;

    void main() {
        fragColor = vec4(vertexColorID, 1.0);
    }
)";

PickingShaderProgram::PickingShaderProgram() : ShaderProgram(vertexSource, fragmentSource) {}

void PickingShaderProgram::setColorID(const glm::vec3 &color) {
    this->setUniformVec3("colorID", color);
}

void PickingShaderProgram::setMatrix(const glm::mat4 &matrix) {
    this->setUniformMat4("modelViewProjectionMatrix", matrix);
}

}
