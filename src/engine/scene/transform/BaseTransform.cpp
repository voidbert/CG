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

#include "engine/scene/transform/BaseTransform.hpp"

namespace engine::scene::transform {

BaseTransform::BaseTransform() : matrix(glm::mat4(1.0f)) {}
BaseTransform::BaseTransform(const glm::mat4 &_matrix) : matrix(_matrix) {}

void BaseTransform::update(float time) {
    static_cast<void>(time);
}

const glm::mat4 &BaseTransform::getMatrix() const {
    return this->matrix;
}

void BaseTransform::draw(render::RenderPipelineManager &pipelineManager,
                         const glm::mat4 &transformMatrix) const {

    static_cast<void>(pipelineManager);
    static_cast<void>(transformMatrix);
}

}
