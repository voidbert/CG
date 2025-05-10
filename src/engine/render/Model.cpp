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

const NormalsPreview &Model::getNormalsPreview() const {
    return this->normalsPreview;
}

void Model::drawSolidColor(RenderPipelineManager &pipelineManager,
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

void Model::drawShaded(RenderPipelineManager &pipelineManager,
                       const glm::mat4 &fullMatrix,
                       const glm::mat4 &worldMatrix,
                       const glm::mat4 &normalMatrix,
                       const std::shared_ptr<Texture> texture,
                       const scene::Material &material) const {

    const ShadedShaderProgram &shader = pipelineManager.getShadedShaderProgram();
    pipelineManager.setFillPolygons(true);
    shader.setFullMatrix(fullMatrix);
    shader.setWorldMatrix(worldMatrix);
    shader.setNormalMatrix(normalMatrix);

    if (texture) {
        texture->use();
        shader.setTexture(*texture);
    } else {
        shader.setMaterial(material);
    }

    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
}

Model::Model(const std::tuple<std::vector<glm::vec4>,
                              std::vector<glm::vec2>,
                              std::vector<glm::vec4>,
                              std::vector<uint32_t>> &modelData) :
    boundingSphere(std::get<0>(modelData)),
    normalsPreview(std::get<0>(modelData), std::get<2>(modelData), std::get<3>(modelData)) {

    const auto &[positions, textureCoordinates, normals, indices] = modelData;

    // Generate buffers
    glGenVertexArrays(1, &this->vao);

    GLuint buffers[4];
    glGenBuffers(4, buffers);
    this->positionsVBO = buffers[0];
    this->textureCoordinatesVBO = buffers[1];
    this->normalsVBO = buffers[2];
    this->ibo = buffers[3];

    // Upload position, texture coordinate, and normal data
    glBindVertexArray(this->vao);
    this->initializeBuffer(0, this->positionsVBO, positions);
    this->initializeBuffer(1, this->textureCoordinatesVBO, textureCoordinates);
    this->initializeBuffer(2, this->normalsVBO, normals);

    // Fill index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(uint32_t),
                 indices.data(),
                 GL_STATIC_DRAW);

    this->vertexCount = indices.size();
}

template<class V>
void Model::initializeBuffer(GLuint attribute, GLuint vbo, const std::vector<V> &data) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(V), data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(attribute, V::length(), GL_FLOAT, false, sizeof(V), nullptr);
    glEnableVertexAttribArray(attribute);
}

}
