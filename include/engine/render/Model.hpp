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

#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <tuple>
#include <vector>

#include "engine/render/BoundingSphere.hpp"
#include "engine/render/NormalsPreview.hpp"
#include "engine/render/RenderPipelineManager.hpp"
#include "engine/render/Texture.hpp"
#include "engine/scene/Material.hpp"
#include "utils/WavefrontOBJ.hpp"

namespace engine::render {

class Model {
private:
    GLuint vao, positionsVBO, textureCoordinatesVBO, normalsVBO, ibo;
    int vertexCount;
    BoundingSphere boundingSphere;
    NormalsPreview normalsPreview;

public:
    explicit Model(const utils::WavefrontOBJ &objectFile);
    Model(const Model &model) = delete;
    Model(Model &&model) = delete;
    ~Model();

    const BoundingSphere &getBoundingSphere() const;
    const NormalsPreview &getNormalsPreview() const;

    void drawSolidColor(RenderPipelineManager &pipelineManager,
                        const glm::mat4 &transformMatrix,
                        const glm::vec4 &color,
                        bool fillPolygons) const;

    void drawShaded(RenderPipelineManager &pipelineManager,
                    const glm::mat4 &fullMatrix,
                    const glm::mat4 &worldMatrix,
                    const glm::mat4 &normalMatrix,
                    const std::shared_ptr<Texture> texture,
                    const scene::Material &material) const;

private:
    explicit Model(const std::tuple<std::vector<glm::vec4>, // Positions
                                    std::vector<glm::vec2>, // Texture coordinates
                                    std::vector<glm::vec4>, // Normals (padded)
                                    std::vector<uint32_t>> // Indices
                       &modelData);

    template<class V>
    void initializeBuffer(GLuint attribute, GLuint vbo, const std::vector<V> &data);
};

}
