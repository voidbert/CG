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
#include <vector>

#include "engine/render/BoundingSphere.hpp"
#include "engine/render/RenderPipeline.hpp"
#include "utils/Vertex.hpp"

namespace engine::render {

class Model; // Importing model would lead to a recursive inclusion

class BoundingSphere {
private:
    glm::vec4 center;
    float radius;

    static std::shared_ptr<Model> sphereModel;
    static bool initializingSphereModel;

public:
    BoundingSphere();
    BoundingSphere(const glm::vec4 &_center, float _radius);
    explicit BoundingSphere(const std::vector<utils::Vertex> &vertices);
    BoundingSphere(const BoundingSphere &sphere, const glm::mat4 &transform);

    glm::vec4 getCenter() const;
    float getRadius() const;

    void draw(const RenderPipeline &pipeline, const glm::mat4 &cameraMatrix) const;
};

}
