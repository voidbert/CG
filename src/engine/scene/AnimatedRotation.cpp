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

#include "engine/scene/AnimatedRotation.hpp"
#include "utils/XMLUtils.hpp"
#include <GLFW/glfw3.h>

namespace engine::scene {

AnimatedRotation::AnimatedRotation(const tinyxml2::XMLElement *rotateElement) {
    this->time = rotateElement->FloatAttribute("time", NAN);
    this->rotationAxis = utils::XMLUtils::getXYZ(rotateElement);

    if (std::isnan(this->time) || this->time <= 0) {
        throw std::runtime_error("Invalid rotation time <rotate> in scene XML file");
    }
}

glm::mat4 AnimatedRotation::getMatrix() const {
    float elapsedTime = glfwGetTime();
    float rotationAngle = (elapsedTime * glm::pi<float>() * 2.0f) / this->time;
    return glm::rotate(rotationAngle, this->rotationAxis);
}

}
