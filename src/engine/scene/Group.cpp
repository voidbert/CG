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

void Group::updateBoundingSphere(const glm::mat4 &worldTransform) {
    const glm::mat4 subTransform = worldTransform * this->transform.getMatrix();

    // Calculate center of group (approximation for objects around the same size)
    glm::vec4 groupCenter(0.0f);

    for (const std::unique_ptr<Entity> &entity : this->entities) {
        entity->calculateBoundingSphere(subTransform);
        groupCenter += entity->getBoundingSphere().getCenter();
    }

    for (const std::unique_ptr<Group> &group : this->groups) {
        group->updateBoundingSphere(subTransform);
        groupCenter += group->boundingSphere.getCenter();
    }

    groupCenter /= this->entities.size() + this->groups.size();

    // Calculate radius
    float radius = 0.0f;
    for (const std::unique_ptr<Entity> &entity : this->entities) {
        const render::BoundingSphere &entitySphere = entity->getBoundingSphere();

        float maxDistance =
            glm::distance(entitySphere.getCenter(), groupCenter) + entitySphere.getRadius();

        if (maxDistance >= radius)
            radius = maxDistance;
    }

    for (const std::unique_ptr<Group> &group : this->groups) {
        float maxDistance = glm::distance(group->boundingSphere.getCenter(), groupCenter) +
            group->boundingSphere.getRadius();

        if (maxDistance >= radius)
            radius = maxDistance;
    }

    this->boundingSphere = render::BoundingSphere(groupCenter, radius);
}

void Group::draw(const render::RenderPipeline &pipeline,
                 const glm::mat4 &cameraMatrix,
                 const glm::mat4 &_transform,
                 bool drawBoundingSpheres) const {

    const glm::mat4 subTransform = _transform * this->transform.getMatrix();

    for (const std::unique_ptr<Entity> &entity : this->entities) {
        entity->draw(pipeline, subTransform);

        if (drawBoundingSpheres)
            entity->getBoundingSphere().draw(pipeline, cameraMatrix);
    }

    for (const std::unique_ptr<Group> &group : this->groups) {
        group->draw(pipeline, cameraMatrix, subTransform, drawBoundingSpheres);
    }

    if (drawBoundingSpheres)
        this->boundingSphere.draw(pipeline, cameraMatrix);
}

}
