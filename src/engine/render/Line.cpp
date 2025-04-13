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

#include "engine/render/Line.hpp"

namespace engine::render {

Line::Line() : vao(0), vbo(0), pointCount(0) {}

Line::Line(const std::vector<utils::Vertex> &points) {
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 points.size() * sizeof(utils::Vertex),
                 points.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    this->pointCount = points.size();
}

void Line::update(const std::vector<utils::Vertex> &points) {
    // Se já existir um VAO ou VBO, apaga
    if (this->vao != 0) {
        glDeleteBuffers(1, &this->vbo);
        glDeleteVertexArrays(1, &this->vao);
    }

    // Criação de novo VAO e VBO
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 points.size() * sizeof(utils::Vertex),
                 points.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    this->pointCount = points.size();
}

Line::~Line() {
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

void Line::draw(const render::RenderPipeline &pipeline,
                const glm::mat4 &cameraMatrix,
                const glm::mat4 &_transform) const {
    pipeline.setMatrix(_transform);
    pipeline.setColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    glBindVertexArray(this->vao);
    glDrawArrays(GL_LINE_STRIP, 0, this->pointCount);
}

}
