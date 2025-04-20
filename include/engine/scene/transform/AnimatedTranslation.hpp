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
#include <glm/vec3.hpp>
#include <memory>
#include <tinyxml2.h>
#include <utility>
#include <vector>

#include "engine/render/LineLoop.hpp"
#include "engine/render/RenderPipelineManager.hpp"
#include "engine/scene/transform/BaseTransform.hpp"

namespace engine::scene::transform {

class AnimatedTranslation : public BaseTransform {
private:
    std::vector<glm::vec3> points;
    glm::vec3 lastUp;
    float translationTime;
    bool align;

    std::unique_ptr<render::LineLoop> line;

public:
    explicit AnimatedTranslation(const tinyxml2::XMLElement *translateElement);

    void update(float time) override;
    void draw(render::RenderPipelineManager &pipelineManager,
              const glm::mat4 &transformMatrix) const override;

private:
    std::pair<glm::vec3, glm::vec3> interpolate(float time);
};

}
