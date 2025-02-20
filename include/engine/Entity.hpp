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

#include <glm/vec4.hpp>
#include <memory>

#include "engine/Model.hpp"
#include "engine/RenderPipeline.hpp"

namespace engine {
class Entity {
protected:
    std::unique_ptr<Model> model;
    glm::vec4 color;
    // TODO - Phase 2 – Add Geometric Transforms

public:
    Entity(std::unique_ptr<Model> argModel, const glm::vec4 &argColor);
    void draw(const RenderPipeline &pipeline) const;
};
}
