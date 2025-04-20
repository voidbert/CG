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

#include <array>
#include <memory>
#include <tinyxml2.h>

#include "engine/render/RenderPipelineManager.hpp"
#include "engine/scene/transform/BaseTransform.hpp"

namespace engine::scene::transform {

class TRSTransform : public BaseTransform {
private:
    std::array<std::unique_ptr<BaseTransform>, 3> transforms;

public:
    TRSTransform();
    explicit TRSTransform(const tinyxml2::XMLElement *transformElement);

    void update(float time) override;
    void draw(render::RenderPipelineManager &pipelineManager,
              const glm::mat4 &transformMatrix) const override;
};

}
