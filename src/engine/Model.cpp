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

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>
#include <unordered_map>

#include "engine/Model.hpp"

namespace engine {
Model::Model(const std::vector<glm::vec4> &positions, const std::vector<std::vector<int>> &faces) {
    // Try to remove repeated vertices from the input (for indexing)
    std::unordered_map<glm::vec4, int> addedVertices;
    std::vector<glm::vec4> vboContents;
    std::vector<int32_t> iboContents;

    for (const std::vector<int> &face : faces) {
        for (const int &faceIndex : face) {
            const glm::vec4 &positionVec = positions.at(faceIndex);

            auto it = addedVertices.find(positionVec);
            int bufferIndex;

            if (it == addedVertices.end()) {
                vboContents.push_back(positionVec);
                bufferIndex = vboContents.size() - 1;
            } else {
                bufferIndex = it->second;
            }

            iboContents.push_back(bufferIndex);
        }
    }

    // Create vertex array and vertex buffers
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vboContents.size() * sizeof(glm::vec4),
                 vboContents.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &this->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 iboContents.size() * sizeof(int32_t),
                 iboContents.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->vertexCount = 3;
}

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
