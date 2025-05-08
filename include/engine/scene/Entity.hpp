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

#include <filesystem>
#include <glm/mat4x4.hpp>
#include <memory>
#include <string>
#include <tinyxml2.h>
#include <unordered_map>

#include "engine/render/BoundingSphere.hpp"
#include "engine/render/Model.hpp"
#include "engine/render/NormalsPreview.hpp"
#include "engine/render/RenderPipelineManager.hpp"
#include "engine/scene/Material.hpp"

namespace engine::scene {

class Entity {
private:
    std::shared_ptr<render::Model> model;
    render::BoundingSphere boundingSphere;
    std::string texturePath;
    Material material;

public:
    Entity(const tinyxml2::XMLElement *modelElement,
           const std::filesystem::path &sceneDirectory,
           std::unordered_map<std::string, std::shared_ptr<render::Model>> &loadedModels);
    Entity(const Entity &entity) = delete;
    Entity(Entity &&entity) = delete;

    void updateBoundingSphere(const glm::mat4 &worldTransform);
    const render::BoundingSphere &getBoundingSphere() const;
    const render::NormalsPreview &getNormalsPreview() const;

    void draw(render::RenderPipelineManager &pipelineManager,
              const glm::mat4 &transformMatrix,
              bool fillPolygons) const;
};

}
