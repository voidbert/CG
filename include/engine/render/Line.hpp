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

#include "engine/render/RenderPipeline.hpp"
#include "utils/Vertex.hpp"

namespace engine::render {

class Line {
private:
    GLuint vao, vbo;
    unsigned int pointCount;

public:
    Line();
    explicit Line(const std::vector<utils::Vertex> &points);
    void update(const std::vector<utils::Vertex> &points);
    ~Line();

    void draw(const render::RenderPipeline &pipeline,
              const glm::mat4 &cameraMatrix,
              const glm::mat4 &_transform) const;
};

}
