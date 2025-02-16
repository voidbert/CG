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
#include <stdexcept>
#include <string>

#include "engine/RenderPipeline.hpp"

namespace engine {
// Having these strings as const char * instead of std::string simplifies OpenGL interop

const char *vertexShaderSource = R"(
#version 460 core
layout (location = 0) in vec4 inPosition;

layout (location = 1) uniform mat4 uniCameraMatrix;

void main() {
    gl_Position = uniCameraMatrix * inPosition;
}
)";

const char *fragmentShaderSource = R"(
#version 460 core
layout (location = 0) out vec4 outColor;

layout (location = 2) uniform vec4 uniColor;

void main() {
    outColor = uniColor;
}
)";

RenderPipeline::RenderPipeline() : vertexShader(0), fragmentShader(0), program(0) {

    // Compile vertex shader
    this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(this->vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(this->vertexShader);
    this->assertShaderCompilation(this->vertexShader);

    // Compile fragment shader
    this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(this->fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(this->fragmentShader);
    this->assertShaderCompilation(this->fragmentShader);

    // Create shader program
    this->program = glCreateProgram();
    glAttachShader(this->program, this->vertexShader);
    glAttachShader(this->program, this->fragmentShader);
    glLinkProgram(this->program);
    this->assertProgramLinking();
}

RenderPipeline::~RenderPipeline() {
    glDeleteShader(this->vertexShader);
    glDeleteShader(this->fragmentShader);
    glDeleteProgram(this->program);
}

void RenderPipeline::use() const {
    glUseProgram(this->program);
    this->setCameraMatrix(glm::mat4(1.0f));
    this->setColor(glm::vec4(1.0f));
}

void RenderPipeline::setCameraMatrix(const glm::mat4 &matrix) const {
    glUniformMatrix4fv(1, 1, false, glm::value_ptr(matrix));
}

void RenderPipeline::setColor(const glm::vec4 &color) const {
    glUniform4f(2, color.x, color.y, color.z, color.w);
}

void RenderPipeline::assertShaderCompilation(GLuint shader) const {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int logLength;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &logLength);

        std::string logMessage;
        logMessage.reserve(logLength + 1);
        glGetShaderInfoLog(shader, logLength, nullptr, logMessage.data());

        throw std::runtime_error("Shader compilation error: " + logMessage);
    }
}

void RenderPipeline::assertProgramLinking() const {
    int success;
    glGetProgramiv(this->program, GL_COMPILE_STATUS, &success);
    if (!success) {
        int logLength;
        glGetProgramiv(this->program, GL_COMPILE_STATUS, &logLength);

        std::string logMessage;
        logMessage.reserve(logLength + 1);
        glGetProgramInfoLog(this->program, logLength, nullptr, logMessage.data());

        throw std::runtime_error("Program linking error: " + logMessage);
    }
}
}
