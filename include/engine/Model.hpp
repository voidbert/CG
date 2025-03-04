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

#include <glad/glad.h>
#include <vector>

#include "utils/WavefrontOBJ.hpp"

namespace engine {
class Model {
private:
    GLuint vao, vbo, ibo;
    unsigned int vertexCount;

public:
    Model(const std::vector<utils::Vertex> &vertices, const std::vector<uint32_t> &indices);
    explicit Model(const utils::WavefrontOBJ &objectFile);

    Model(const Model &model) = delete;
    Model(Model &&) = delete;
    ~Model();

    void draw() const;

private:
    explicit Model(const std::pair<std::vector<utils::Vertex>, std::vector<uint32_t>> &vertices);
};
}
