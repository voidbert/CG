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

#include <numeric>
#include <stdexcept>

#include "engine/scene/Group.hpp"

namespace engine::scene {

Group::Group(const tinyxml2::XMLElement *groupElement,
             const std::filesystem::path &sceneDirectory,
             std::unordered_map<std::string, std::shared_ptr<render::Model>> &loadedModels,
             std::unordered_map<std::string, std::shared_ptr<render::Texture>> &loadedTextures) {

    // Parse entities
    const tinyxml2::XMLElement *modelsElement = groupElement->FirstChildElement("models");
    if (modelsElement) {
        if (modelsElement->NextSiblingElement("models")) {
            throw std::runtime_error("More than one <models> element in scene XML");
        }

        const tinyxml2::XMLElement *modelElement = modelsElement->FirstChildElement("model");
        while (modelElement) {
            this->entities.push_back(std::make_unique<Entity>(modelElement,
                                                              sceneDirectory,
                                                              loadedModels,
                                                              loadedTextures));
            modelElement = modelElement->NextSiblingElement("model");
        }
    }

    // Parse subgroups
    const tinyxml2::XMLElement *innerGroupElement = groupElement->FirstChildElement("group");
    while (innerGroupElement) {
        this->groups.push_back(std::make_unique<Group>(innerGroupElement,
                                                       sceneDirectory,
                                                       loadedModels,
                                                       loadedTextures));
        innerGroupElement = innerGroupElement->NextSiblingElement("group");
    }

    // Parse transform
    const tinyxml2::XMLElement *transformElement = groupElement->FirstChildElement("transform");
    if (transformElement) {
        transform = transform::TRSTransform(transformElement);
    }
}

int Group::getEntityCount() const {
    return std::transform_reduce(
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
    groupCenter += this->sumBoundingSphereCenters(this->entities, subTransform);
    groupCenter += this->sumBoundingSphereCenters(this->groups, subTransform);
    groupCenter /= this->entities.size() + this->groups.size();

    // Calculate radius of the sphere
    const float entitiesRadius = this->calculateBoundingSphereRadius(this->entities, groupCenter);
    const float groupsRadius = this->calculateBoundingSphereRadius(this->groups, groupCenter);
    const float radius = std::max(entitiesRadius, groupsRadius);

    this->boundingSphere = render::BoundingSphere(groupCenter, radius);
}

void Group::update(float time) {
    this->transform.update(time);
    for (const std::unique_ptr<Group> &group : this->groups) {
        group->update(time);
    }
}

int Group::draw(render::RenderPipelineManager &pipelineManager,
                const camera::Camera &camera,
                const glm::mat4 &_transform,
                bool fillPolygons,
                bool showBoundingSpheres,
                bool showAnimationLines,
                bool showNormals) const {

    const glm::mat4 &cameraMatrix = camera.getCameraMatrix();

    if (showAnimationLines) {
        this->transform.draw(pipelineManager, _transform);
    }

    const glm::mat4 subTransform = _transform * this->transform.getMatrix();
    int renderedEntities = 0;

    if (!camera.isInFrustum(this->boundingSphere))
        return renderedEntities;

    for (const std::unique_ptr<Entity> &entity : this->entities) {
        const render::BoundingSphere entityBoundingSphere = entity->getBoundingSphere();

        if (camera.isInFrustum(entityBoundingSphere)) {
            entity->draw(pipelineManager, subTransform, fillPolygons);
            renderedEntities++; // cppcheck-suppress useStlAlgorithm

            if (showBoundingSpheres) {
                entityBoundingSphere.draw(pipelineManager,
                                          cameraMatrix,
                                          glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            }

            if (showNormals) {
                entity->getNormalsPreview().draw(pipelineManager,
                                                 subTransform,
                                                 glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
            }
        }
    }

    for (const std::unique_ptr<Group> &group : this->groups) {
        // cppcheck-suppress useStlAlgorithm
        renderedEntities += group->draw(pipelineManager,
                                        camera,
                                        subTransform,
                                        fillPolygons,
                                        showBoundingSpheres,
                                        showAnimationLines,
                                        showNormals);
    }

    if (showBoundingSpheres) {
        this->boundingSphere.draw(pipelineManager, cameraMatrix, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    return renderedEntities;
}

const render::BoundingSphere &Group::getBoundingSphere() const {
    return this->boundingSphere;
}

template<class T>
glm::vec4 Group::sumBoundingSphereCenters(const std::vector<std::unique_ptr<T>> &ts,
                                          const glm::mat4 &subTransform) {

    return std::transform_reduce(ts.cbegin(),
                                 ts.cend(),
                                 glm::vec4(0.0f),
                                 std::plus<>(),
                                 [subTransform](const std::unique_ptr<T> &t) {
                                     t->updateBoundingSphere(subTransform);
                                     return t->getBoundingSphere().getCenter();
                                 });
}

template<class T>
float Group::calculateBoundingSphereRadius(const std::vector<std::unique_ptr<T>> &ts,
                                           const glm::vec4 &groupCenter) {

    return std::transform_reduce(
        ts.cbegin(),
        ts.cend(),
        0.0f,
        [](float d1, float d2) { return std::max(d1, d2); },
        [groupCenter](const std::unique_ptr<T> &t) {
            const render::BoundingSphere &sphere = t->getBoundingSphere();
            return glm::distance(sphere.getCenter(), groupCenter) + sphere.getRadius();
        });
}

}
