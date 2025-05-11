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

#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "engine/scene/light/DirectionalLight.hpp"
#include "engine/scene/light/LightFactory.hpp"
#include "engine/scene/light/PointLight.hpp"
#include "engine/scene/light/Spotlight.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::scene::light {

std::unique_ptr<Light> LightFactory::createFromXML(const tinyxml2::XMLElement *lightElement) {
    std::string lightType = "directional";
    const char *lightTypePtr = lightElement->Attribute("type");
    if (lightTypePtr) {
        lightType = lightTypePtr;
    }

    if (lightType == "directional") {
        const glm::vec3 direction = utils::XMLUtils::getLightDirection(lightElement);
        return std::make_unique<light::DirectionalLight>(direction);

    } else if (lightType == "point") {
        const glm::vec3 position = utils::XMLUtils::getLightPosition(lightElement);
        return std::make_unique<light::PointLight>(position);

    } else if (lightType == "spotlight") {
        const glm::vec3 position = utils::XMLUtils::getLightPosition(lightElement);
        const glm::vec3 direction = utils::XMLUtils::getLightDirection(lightElement);
        const float cutoff = glm::radians(lightElement->FloatAttribute("cutoff", 10.0f));

        return std::make_unique<light::Spotlight>(position, direction, cutoff);
    } else {
        throw std::runtime_error("Invalid light type in scene XML file");
    }
}

}
