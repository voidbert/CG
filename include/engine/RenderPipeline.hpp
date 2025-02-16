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

namespace engine {
class RenderPipeline {
private:
    GLuint vertexShader, fragmentShader, program;

public:
    RenderPipeline();
    RenderPipeline(const RenderPipeline &model) = delete;
    RenderPipeline(RenderPipeline &&) = delete;
    ~RenderPipeline();

    void use() const;
    // void setCameraMatrix(const glm::mat4 &matrix) const;
    void setTransformMatrix(const glm::mat4 &matrix) const;
    void setColor(const glm::vec4 &color) const;

private:
    void assertShaderCompilation(GLuint shader) const;
    void assertProgramLinking() const;
};
}
