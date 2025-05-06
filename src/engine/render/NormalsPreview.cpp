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

#include "engine/render/NormalsPreview.hpp"

namespace engine::render {

NormalsPreview::NormalsPreview(const std::vector<glm::vec4> &positions,
                               const std::vector<glm::vec4> &normals,
                               const std::vector<uint32_t> &indices) {

    // Create vertex data
    std::vector<glm::vec4> points;
    points.reserve(indices.size() * 2);

    for (uint32_t index : indices) {
        const glm::vec4 position = positions[index];
        const glm::vec4 normal = normals[index];

        points.push_back(position);
        points.push_back(position + normal);
    }

    // Create buffers
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    // Fill vertex data
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec4), points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(glm::vec4), nullptr);
    glEnableVertexAttribArray(0);

    this->vertexCount = points.size();
}

NormalsPreview::~NormalsPreview() {
    glDeleteBuffers(1, &this->vao);
    glDeleteVertexArrays(1, &this->vao);
}

void NormalsPreview::draw(RenderPipelineManager &pipelineManager,
                          const glm::mat4 &transformMatrix,
                          const glm::vec4 &color) const {

    const SolidColorShaderProgram &shader = pipelineManager.getSolidColorShaderProgram();
    shader.setMatrix(transformMatrix);
    shader.setColor(color);

    glBindVertexArray(this->vao);
    glDrawArrays(GL_LINES, 0, this->vertexCount);
}

}
