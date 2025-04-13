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
#include <glm/mat4x4.hpp>
#include <memory>
#include <tinyxml2.h>

#include "engine/render/Line.hpp"
#include "engine/render/RenderPipeline.hpp"
#include "engine/scene/ITransform.hpp"
#include "utils/Vertex.hpp"

namespace engine::scene {

class TRSTransform : public ITransform {
private:
    std::array<std::unique_ptr<ITransform>, 3> transformations;
    render::Line catmullRomMotionLine;
    int iAnimatedTranslation = -1;
    std::vector<glm::vec3> catmullRomPoints;

public:
    TRSTransform();
    explicit TRSTransform(const tinyxml2::XMLElement *transformElement);
    glm::mat4 getMatrix() const override;
    int draw(const render::RenderPipeline &pipeline,
             const glm::mat4 &cameraMatrix,
             const glm::mat4 &_transform);
};

}
