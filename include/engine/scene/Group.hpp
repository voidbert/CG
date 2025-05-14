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
#include <glm/vec4.hpp>
#include <memory>
#include <string>
#include <tinyxml2.h>
#include <unordered_map>
#include <vector>

#include "engine/render/BoundingSphere.hpp"
#include "engine/render/Model.hpp"
#include "engine/render/RenderPipelineManager.hpp"
#include "engine/render/Texture.hpp"
#include "engine/scene/camera/Camera.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/scene/transform/TRSTransform.hpp"

namespace engine::scene {

class Group {
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Group>> groups;
    render::BoundingSphere boundingSphere;
    transform::TRSTransform transform;

public:
    Group(const tinyxml2::XMLElement *groupElement,
          const std::filesystem::path &sceneDirectory,
          std::unordered_map<std::string, std::shared_ptr<render::Model>> &loadedModels,
          std::unordered_map<std::string, std::shared_ptr<render::Texture>> &loadedTextures);
    Group(const Group &group) = delete;
    Group(Group &&group) = delete;

    int getEntityCount() const;

    void update(const glm::mat4 &worldTransform, float time);

    void drawSolidColorParts(render::RenderPipelineManager &pipelineManager,
                             const camera::Camera &camera,
                             const glm::mat4 &worldTransform,
                             bool showBoundingSpheres,
                             bool showAnimationLines,
                             bool showNormals) const;
    int drawShadedParts(render::RenderPipelineManager &pipelineManager,
                        const camera::Camera &camera,
                        const glm::mat4 &worldtransform,
                        bool fillPolygons) const;

    int drawForPicking(render::RenderPipelineManager &pipelineManager,
                       const camera::Camera &camera,
                       const glm::mat4 &worldTransform,
                       int currentId,
                       std::unordered_map<int, std::string> *idToNameMap = nullptr) const;

private:
    const render::BoundingSphere &getBoundingSphere() const;
    void updateBoundingSphere(const glm::mat4 &worldTransform);

    template<class T>
    glm::vec4 sumBoundingSphereCenters(const std::vector<std::unique_ptr<T>> &ts,
                                       const glm::mat4 &subTransform);

    template<class T>
    float calculateBoundingSphereRadius(const std::vector<std::unique_ptr<T>> &ts,
                                        const glm::vec4 &groupCenter);
};

}
