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
             std::unordered_map<std::string, std::shared_ptr<Model>> &loadedModels) {

    // Parse entities
    const tinyxml2::XMLElement *modelsElement = groupElement->FirstChildElement("models");
    if (!modelsElement) {
        return; // <models> is optional
    }

    if (modelsElement->NextSiblingElement("models")) {
        throw std::runtime_error("More than one <models> element in scene XML");
    }

    const tinyxml2::XMLElement *modelElement = modelsElement->FirstChildElement("model");
    while (modelElement) {
        this->entities.push_back(
            std::make_unique<Entity>(modelElement, sceneDirectory, loadedModels));
        modelElement = modelElement->NextSiblingElement("model");
    }

    // Parse subgroups
    const tinyxml2::XMLElement *innerGroupElement = groupElement->FirstChildElement("group");
    while (innerGroupElement) {
        this->groups.push_back(
            std::make_unique<Group>(innerGroupElement, sceneDirectory, loadedModels));
        innerGroupElement = innerGroupElement->NextSiblingElement("group");
    }
}

void Group::draw(const RenderPipeline &pipeline) const {
    for (const std::unique_ptr<Entity> &entity : this->entities) {
        entity->draw(pipeline);
    }

    for (const std::unique_ptr<Group> &group : this->groups) {
        group->draw(pipeline);
    }
}

}
