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

#include "engine/RenderPipeline.hpp"

namespace engine {
// Having these strings as const char * instead of std::string simplifies OpenGL interop

const char *vertexShaderSource = R"(
#version 460 core
layout (location = 0) in vec4 position;

void main()
{
    gl_Position = position;
}
)";

const char *fragmentShaderSource = R"(
#version 460 core
out vec4 fragColor;

void main()
{
    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

RenderPipeline::RenderPipeline() {
    // TODO - add error handling

    this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(this->vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(this->fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    this->program = glCreateProgram();
    glAttachShader(this->program, this->vertexShader);
    glAttachShader(this->program, this->vertexShader);
    glLinkProgram(this->program);
}

RenderPipeline::~RenderPipeline() {
    glDeleteShader(this->vertexShader);
    glDeleteShader(this->fragmentShader);
    glDeleteProgram(this->program);
}

void RenderPipeline::use() const {
    glUseProgram(this->program);
}
}
