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
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <vector>

#include "engine/render/RenderPipelineManager.hpp"

namespace engine::render {

class LineLoop {
private:
    GLuint vao, vbo;
    int pointCount;

public:
    explicit LineLoop(const std::vector<glm::vec4> &points);
    LineLoop(const LineLoop &lineLoop) = delete;
    LineLoop(LineLoop &&lineLoop) = delete;
    ~LineLoop();

    void draw(RenderPipelineManager &pipelineManager,
              const glm::mat4 &fullMatrix,
              const glm::vec4 &color) const;
};

}
