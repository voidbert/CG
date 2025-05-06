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

#include "engine/scene/Entity.hpp"

#include "utils/WavefrontOBJ.hpp"

namespace engine::scene {

Entity::Entity(const tinyxml2::XMLElement *modelElement,
               const std::filesystem::path &sceneDirectory,
               std::unordered_map<std::string, std::shared_ptr<render::Model>> &loadedModels) {

    const char *file = modelElement->Attribute("file");
    if (!file) {
        throw std::runtime_error("<rotate> missing file attribute in scene XML file");
    }

    const std::string modelPath = std::filesystem::canonical(sceneDirectory / file);
    auto it = loadedModels.find(modelPath);
    if (it == loadedModels.end()) {
        utils::WavefrontOBJ object(modelPath);
        this->model = std::make_shared<render::Model>(object);
        loadedModels[modelPath] = model;
    } else {
        this->model = it->second;
    }
}

void Entity::updateBoundingSphere(const glm::mat4 &worldTransform) {
    this->boundingSphere = render::BoundingSphere(this->model->getBoundingSphere(), worldTransform);
}

const render::BoundingSphere &Entity::getBoundingSphere() const {
    return this->boundingSphere;
}

const render::NormalsPreview &Entity::getNormalsPreview() const {
    return this->model->getNormalsPreview();
}

void Entity::draw(render::RenderPipelineManager &pipelineManager,
                  const glm::mat4 &transformMatrix,
                  bool fillPolygons) const {

    this->model->draw(pipelineManager, transformMatrix, glm::vec4(1.0f), fillPolygons);
}

}
