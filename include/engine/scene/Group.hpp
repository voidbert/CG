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
#include <vector>

#include "engine/camera/Camera.hpp"
#include "engine/render/BoundingSphere.hpp"
#include "engine/render/RenderPipeline.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/scene/TRSTransform.hpp"

namespace engine::scene {

class Group {
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Group>> groups;
    render::BoundingSphere boundingSphere;
    TRSTransform transform;

public:
    Group(const tinyxml2::XMLElement *groupElement,
          const std::filesystem::path &sceneDirectory,
          std::unordered_map<std::string, std::shared_ptr<render::Model>> &loadedModels);
    Group(const Group &entity) = delete;
    Group(Group &&entity) = delete;

    int getEntityCount() const;
    void updateBoundingSphere(const glm::mat4 &worldTransform);

    int draw(const render::RenderPipeline &pipeline,
             const camera::Camera &camera,
             const glm::mat4 &_transform,
             bool drawBoundingSpheres,
             bool drawCatmullRomMotionLines) const;
};

}
