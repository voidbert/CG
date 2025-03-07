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

#include <glm/vec4.hpp>

#include "engine/render/Axis.hpp"

namespace engine::render {

Axis::Axis(const glm::vec3 &direction) {
    const glm::vec3 normalized = glm::normalize(direction);
    glm::vec4 vertices[] = { glm::vec4(normalized * 1000.0f, 1.0f),
                             glm::vec4(normalized * -1000.0f, 1.0f) };

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec4), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    this->color = glm::vec4(normalized, 1.0f);
}

Axis::~Axis() {
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

void Axis::draw(const RenderPipeline &pipeline) const {
    pipeline.setColor(this->color);
    glBindVertexArray(this->vao);
    glDrawArrays(GL_LINES, 0, 2);
}

}
