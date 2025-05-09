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

#include <stdexcept>

#include "engine/render/ShaderProgram.hpp"

namespace engine::render {

ShaderProgram::ShaderProgram(const std::string &vertexShaderSource,
                             const std::string &fragmentShaderSource) {
    // Compile shaders
    this->vertexShader = this->compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    this->fragmentShader = this->compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Link program
    this->program = glCreateProgram();
    glAttachShader(this->program, this->vertexShader);
    glAttachShader(this->program, this->fragmentShader);
    glLinkProgram(this->program);

    GLint success;
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &logLength);

        std::string logMessage;
        logMessage.reserve(logLength + 1);
        glGetProgramInfoLog(this->program, logLength, nullptr, logMessage.data());

        throw std::runtime_error("Shader linking error: " + logMessage);
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteShader(this->vertexShader);
    glDeleteShader(this->fragmentShader);
    glDeleteProgram(this->program);
}

void ShaderProgram::use() const {
    glUseProgram(this->program);
}

GLuint ShaderProgram::compileShader(GLenum type, const std::string &source) {
    const GLuint shader = glCreateShader(type);
    const char *rawSource = source.c_str();
    glShaderSource(shader, 1, &rawSource, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::string logMessage;
        logMessage.reserve(logLength + 1);
        glGetShaderInfoLog(shader, logLength, nullptr, logMessage.data());

        throw std::runtime_error("Shader compilation error: " + logMessage);
    }

    return shader;
}

}
