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

#include <execution>
#include <numeric>
#include <stdexcept>

#include "engine/scene/Group.hpp"

namespace engine::scene {

Group::Group(const tinyxml2::XMLElement *groupElement,
             const std::filesystem::path &sceneDirectory,
             std::unordered_map<std::string, std::shared_ptr<render::Model>> &loadedModels) {

    // Parse entities
    const tinyxml2::XMLElement *modelsElement = groupElement->FirstChildElement("models");
    if (modelsElement) {
        if (modelsElement->NextSiblingElement("models")) {
            throw std::runtime_error("More than one <models> element in scene XML");
        }

        const tinyxml2::XMLElement *modelElement = modelsElement->FirstChildElement("model");
        while (modelElement) {
            this->entities.push_back(
                std::make_unique<Entity>(modelElement, sceneDirectory, loadedModels));
            modelElement = modelElement->NextSiblingElement("model");
        }
    }

    // Parse subgroups
    const tinyxml2::XMLElement *innerGroupElement = groupElement->FirstChildElement("group");
    while (innerGroupElement) {
        this->groups.push_back(
            std::make_unique<Group>(innerGroupElement, sceneDirectory, loadedModels));
        innerGroupElement = innerGroupElement->NextSiblingElement("group");
    }

    // Parse transform
    const tinyxml2::XMLElement *transformElement = groupElement->FirstChildElement("transform");
    if (transformElement) {
        transform = TRSTransform(transformElement);
    }
}

int Group::getEntityCount() const {
    return std::transform_reduce(
        std::execution::par,
        this->groups.cbegin(),
        this->groups.cend(),
        this->entities.size(),
        std::plus<>(),
        [](const std::unique_ptr<Group> &group) { return group->getEntityCount(); });
}

void Group::updateBoundingSphere(const glm::mat4 &worldTransform) {
    const glm::mat4 subTransform = worldTransform * this->transform.getMatrix();

    // Calculate center of group (approximation for objects around the same size)
    glm::vec4 groupCenter(0.0f);

    groupCenter += std::transform_reduce(std::execution::par,
                                         this->entities.cbegin(),
                                         this->entities.cend(),
                                         glm::vec4(0.0f),
                                         std::plus<>(),
                                         [subTransform](const std::unique_ptr<Entity> &entity) {
                                             entity->updateBoundingSphere(subTransform);
                                             return entity->getBoundingSphere().getCenter();
                                         });

    groupCenter += std::transform_reduce(std::execution::par,
                                         this->groups.cbegin(),
                                         this->groups.cend(),
                                         glm::vec4(0.0f),
                                         std::plus<>(),
                                         [subTransform](const std::unique_ptr<Group> &group) {
                                             group->updateBoundingSphere(subTransform);
                                             return group->boundingSphere.getCenter();
                                         });

    groupCenter /= this->entities.size() + this->groups.size();

    const float entitiesRadius = std::transform_reduce(
        std::execution::par,
        this->entities.cbegin(),
        this->entities.cend(),
        0.0f,
        [](float d1, float d2) { return std::max(d1, d2); },
        [groupCenter](const std::unique_ptr<Entity> &entity) {
            const render::BoundingSphere &entitySphere = entity->getBoundingSphere();
            return glm::distance(entitySphere.getCenter(), groupCenter) + entitySphere.getRadius();
        });

    const float groupsRadius = std::transform_reduce(
        std::execution::par,
        this->groups.cbegin(),
        this->groups.cend(),
        0.0f,
        [](float d1, float d2) { return std::max(d1, d2); },
        [groupCenter](const std::unique_ptr<Group> &group) {
            const render::BoundingSphere &groupSphere = group->boundingSphere;
            return glm::distance(groupSphere.getCenter(), groupCenter) + groupSphere.getRadius();
        });

    const float radius = std::max(entitiesRadius, groupsRadius);
    this->boundingSphere = render::BoundingSphere(groupCenter, radius);
}

int Group::draw(const render::RenderPipeline &pipeline,
                const camera::Camera &camera,
                const glm::mat4 &_transform,
                bool drawBoundingSpheres) const {

    const glm::mat4 subTransform = _transform * this->transform.getMatrix();
    const glm::mat4 &cameraMatrix = camera.getCameraMatrix();
    int renderedEntities = 0;

    if (!camera.isInFrustum(this->boundingSphere))
        return renderedEntities;

    for (const std::unique_ptr<Entity> &entity : this->entities) {
        const render::BoundingSphere entityBoundingSphere = entity->getBoundingSphere();

        if (camera.isInFrustum(entityBoundingSphere)) {
            entity->draw(pipeline, subTransform);
            renderedEntities++; // cppcheck-suppress useStlAlgorithm

            if (drawBoundingSpheres)
                entityBoundingSphere.draw(pipeline, cameraMatrix);
        }
    }

    for (const std::unique_ptr<Group> &group : this->groups) {
        // cppcheck-suppress useStlAlgorithm
        renderedEntities += group->draw(pipeline, camera, subTransform, drawBoundingSpheres);
    }

    if (drawBoundingSpheres)
        this->boundingSphere.draw(pipeline, cameraMatrix);

    return renderedEntities;
}

}
