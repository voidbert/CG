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

#include <unordered_map>

#include "engine/Model.hpp"

namespace engine {

Model::Model(const std::vector<utils::Vertex> &vertices, const std::vector<uint32_t> &indices) {
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(utils::Vertex),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &this->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(GLuint),
                 indices.data(),
                 GL_STATIC_DRAW);

    this->vertexCount = indices.size();
}

Model::Model(const utils::WavefrontOBJ &objectFile) : Model(objectFile.getIndexedVertices()) {}

Model::Model(const std::pair<std::vector<utils::Vertex>, std::vector<uint32_t>> &vertices) :
    Model(vertices.first, vertices.second) {}

Model::~Model() {
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(1, &this->ibo);
    glDeleteVertexArrays(1, &this->vao);
}

void Model::draw() const {
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
}

}
