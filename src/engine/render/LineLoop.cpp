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

#include "engine/render/LineLoop.hpp"

#include "engine/render/SolidColorShaderProgram.hpp"

namespace engine::render {

LineLoop::LineLoop(const std::vector<glm::vec4> &points) {
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec4), points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), nullptr);
    glEnableVertexAttribArray(0);

    this->pointCount = points.size();
}

LineLoop::~LineLoop() {
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

void LineLoop::draw(RenderPipelineManager &pipelineManager,
                    const glm::mat4 &fullMatrix,
                    const glm::vec4 &color) const {

    const SolidColorShaderProgram &shader = pipelineManager.getSolidColorShaderProgram();
    shader.setFullMatrix(fullMatrix);
    shader.setColor(color);

    glBindVertexArray(this->vao);
    glDrawArrays(GL_LINE_LOOP, 0, this->pointCount);
}

}
