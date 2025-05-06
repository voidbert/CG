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

#include "engine/render/Model.hpp"

namespace engine::render {

Model::Model(const utils::WavefrontOBJ &objectFile) : Model(objectFile.getIndexedVertices()) {}

Model::Model(const std::tuple<std::vector<glm::vec4>,
                              std::vector<glm::vec2>,
                              std::vector<glm::vec4>,
                              std::vector<uint32_t>> &modelData) :
    boundingSphere(std::get<0>(modelData)) {

    const auto &[positions, textureCoordinates, normals, indices] = modelData;

    // Generate buffers
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    GLuint buffers[4];
    glGenBuffers(4, buffers);
    this->positionsVBO = buffers[0];
    this->textureCoordinatesVBO = buffers[1];
    this->normalsVBO = buffers[2];
    this->ibo = buffers[3];

    // Fill position, texture coordinate, and normal data
    glBindBuffer(GL_ARRAY_BUFFER, this->positionsVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 positions.size() * sizeof(glm::vec4),
                 positions.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(glm::vec4), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 textureCoordinates.size() * sizeof(glm::vec2),
                 textureCoordinates.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 normals.size() * sizeof(glm::vec4),
                 normals.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(glm::vec4), nullptr);
    glEnableVertexAttribArray(2);

    // Fill index data
    glGenBuffers(1, &this->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(GLuint),
                 indices.data(),
                 GL_STATIC_DRAW);

    this->vertexCount = indices.size();
}

Model::~Model() {
    GLuint buffers[4] = { this->positionsVBO,
                          this->textureCoordinatesVBO,
                          this->normalsVBO,
                          this->ibo };

    glDeleteBuffers(4, buffers);
    glDeleteVertexArrays(1, &this->vao);
}

const BoundingSphere &Model::getBoundingSphere() const {
    return this->boundingSphere;
}

void Model::draw(RenderPipelineManager &pipelineManager,
                 const glm::mat4 &transformMatrix,
                 const glm::vec4 &color,
                 bool fillPolygons) const {

    const SolidColorShaderProgram &shader = pipelineManager.getSolidColorShaderProgram();
    pipelineManager.setFillPolygons(fillPolygons);
    shader.setMatrix(transformMatrix);
    shader.setColor(color);

    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
}

}
