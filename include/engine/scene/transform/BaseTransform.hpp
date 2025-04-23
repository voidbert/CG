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

#include <glm/mat4x4.hpp>

#include "engine/render/RenderPipelineManager.hpp"

namespace engine::scene::transform {

class BaseTransform {
protected:
    glm::mat4 matrix;

public:
    BaseTransform();
    explicit BaseTransform(const glm::mat4 &_matrix);

    virtual void update(float time);
    virtual const glm::mat4 &getMatrix() const;
    virtual void draw(render::RenderPipelineManager &pipelineManager,
                      const glm::mat4 &transformMatrix) const;
};

}
