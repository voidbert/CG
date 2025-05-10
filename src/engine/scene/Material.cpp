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

#include "engine/scene/Material.hpp"

#include "utils/XMLUtils.hpp"

namespace engine::scene {

Material::Material() :
    diffuse(glm::vec3(1.0f)),
    ambient(glm::vec3(0.2f)),
    specular(glm::vec3(0.0f)),
    emissive(glm::vec3(0.0f)),
    shininess(0.0f) {}

Material::Material(const tinyxml2::XMLElement *colorElement) {
    this->diffuse =
        utils::XMLUtils::getRGB(utils::XMLUtils::getSingleChild(colorElement, "diffuse"));
    this->ambient =
        utils::XMLUtils::getRGB(utils::XMLUtils::getSingleChild(colorElement, "ambient"));
    this->specular =
        utils::XMLUtils::getRGB(utils::XMLUtils::getSingleChild(colorElement, "specular"));
    this->emissive =
        utils::XMLUtils::getRGB(utils::XMLUtils::getSingleChild(colorElement, "emissive"));

    const tinyxml2::XMLElement *shininessElement = colorElement->FirstChildElement("shininess");
    if (shininessElement) {
        this->shininess = shininessElement->FloatAttribute("value", this->shininess);
    }
}

const glm::vec3 &Material::getDiffuse() const {
    return this->diffuse;
}

const glm::vec3 &Material::getAmbient() const {
    return this->ambient;
}

const glm::vec3 &Material::getSpecular() const {
    return this->specular;
}

const glm::vec3 &Material::getEmissive() const {
    return this->emissive;
}

float Material::getShininess() const {
    return this->shininess;
}

}
