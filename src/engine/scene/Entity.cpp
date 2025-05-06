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
#include "utils/XMLUtils.hpp"

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

    // Optional texture
    const tinyxml2::XMLElement *textureElement = modelElement->FirstChildElement("texture");
    if (textureElement) {
        const char *texFile = textureElement->Attribute("file");
        if (texFile) {
            this->texturePath = (sceneDirectory / texFile).string();
        }
    }

    // Optional material
    const tinyxml2::XMLElement *colorElement = modelElement->FirstChildElement("color");
    if (colorElement) {
        this->material.diffuse =
            utils::XMLUtils::getRGB(colorElement->FirstChildElement("diffuse"));
        this->material.ambient =
            utils::XMLUtils::getRGB(colorElement->FirstChildElement("ambient"));
        this->material.specular =
            utils::XMLUtils::getRGB(colorElement->FirstChildElement("specular"));
        this->material.emissive =
            utils::XMLUtils::getRGB(colorElement->FirstChildElement("emissive"));

        const tinyxml2::XMLElement *shininessElement = colorElement->FirstChildElement("shininess");
        if (shininessElement) {
            this->material.shininess =
                shininessElement->FloatAttribute("value", this->material.shininess);
        }
    }
}

void Entity::updateBoundingSphere(const glm::mat4 &worldTransform) {
    this->boundingSphere = render::BoundingSphere(this->model->getBoundingSphere(), worldTransform);
}

const render::BoundingSphere &Entity::getBoundingSphere() const {
    return this->boundingSphere;
}

void Entity::draw(render::RenderPipelineManager &pipelineManager,
                  const glm::mat4 &transformMatrix,
                  bool fillPolygons) const {

    this->model->draw(pipelineManager, transformMatrix, glm::vec4(1.0f), fillPolygons);
}

}
