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
               std::unordered_map<std::string, std::shared_ptr<render::Model>> &loadedModels,
               std::unordered_map<std::string, std::shared_ptr<render::Texture>> &loadedTextures) {

    // Get model
    const char *file = modelElement->Attribute("file");
    if (!file) {
        throw std::runtime_error("<rotate> missing file attribute in scene XML file");
    }

    const char *nameAttr = modelElement->Attribute("name");
    this->name = nameAttr ? std::string(nameAttr) : "UnnamedEntity";

    const std::string modelPath = std::filesystem::canonical(sceneDirectory / file);
    auto modelIt = loadedModels.find(modelPath);
    if (modelIt == loadedModels.end()) {
        utils::WavefrontOBJ object(modelPath);
        this->model = std::make_shared<render::Model>(object);
        loadedModels[modelPath] = model;
    } else {
        this->model = modelIt->second;
    }

    // Optional texture
    const tinyxml2::XMLElement *textureElement = modelElement->FirstChildElement("texture");
    if (textureElement) {
        const char *textureFile = textureElement->Attribute("file");
        if (textureFile) {
            const std::string texturePath =
                std::filesystem::canonical(sceneDirectory / textureFile);

            auto textureIt = loadedTextures.find(texturePath);
            if (textureIt == loadedTextures.end()) {
                this->texture = std::make_shared<render::Texture>(texturePath);
                loadedTextures[texturePath] = texture;
            } else {
                this->texture = textureIt->second;
            }
        }
    }

    // Optional material
    const tinyxml2::XMLElement *colorElement = modelElement->FirstChildElement("color");
    if (colorElement) {
        this->material = Material(colorElement);
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

const std::string &Entity::getName() const {
    return this->name;
}

void Entity::drawSolidColor(render::RenderPipelineManager &pipelineManager,
                            const glm::mat4 &fullMatrix,
                            const glm::vec4 &color,
                            bool fillPolygons) const {

    this->model->drawSolidColor(pipelineManager, fullMatrix, color, fillPolygons);
}

void Entity::draw(render::RenderPipelineManager &pipelineManager,
                  const glm::mat4 &fullMatrix,
                  const glm::mat4 &worldMatrix,
                  const glm::mat4 &normalMatrix,
                  bool fillPolygons) const {

    if (fillPolygons) {
        this->model->drawShaded(pipelineManager,
                                fullMatrix,
                                worldMatrix,
                                normalMatrix,
                                this->texture,
                                this->material);
    } else {
        this->drawSolidColor(pipelineManager, fullMatrix, glm::vec4(1.0f), false);
    }
}

}
