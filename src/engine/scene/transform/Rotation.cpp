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
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <stdexcept>

#include "engine/scene/transform/Rotation.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::scene::transform {

Rotation::Rotation(const tinyxml2::XMLElement *rotateElement) {
    const float rotationAngle = glm::radians(rotateElement->FloatAttribute("angle", NAN));
    const glm::vec3 rotationAxis = utils::XMLUtils::getXYZ(rotateElement);

    if (std::isnan(rotationAngle)) {
        throw std::runtime_error("<rotate> missing angle attribute in scene XML file");
    }

    this->matrix = glm::rotate(rotationAngle, rotationAxis);
}

}
