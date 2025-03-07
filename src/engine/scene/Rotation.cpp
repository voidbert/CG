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

#include <cmath>
#include <glm/gtx/transform.hpp>
#include <stdexcept>

#include "engine/scene/Rotation.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::scene {

Rotation::Rotation(const tinyxml2::XMLElement *rotateElement) {
    this->rotationAngle = rotateElement->FloatAttribute("angle", NAN);
    this->rotationAxis = utils::XMLUtils::getXYZ(rotateElement);

    if (std::isnan(this->rotationAngle)) {
        throw std::runtime_error("Invalid rotation angle <rotate> in scene XML file");
    } else {
        this->rotationAngle *= (glm::pi<float>() / 180.0f); // Convert to radians
    }
}

glm::mat4 Rotation::getMatrix() const {
    return glm::rotate(this->rotationAngle, this->rotationAxis);
}

}
