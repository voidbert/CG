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
#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>
#include <stdexcept>

#include "engine/scene/transform/AnimatedRotation.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::scene::transform {

AnimatedRotation::AnimatedRotation(const tinyxml2::XMLElement *rotateElement) {
    this->rotationTime = rotateElement->FloatAttribute("time", NAN);
    if (std::isnan(this->rotationTime)) {
        throw std::runtime_error("<rotate> missing time attribute in scene XML file");
    }

    this->rotationAxis = utils::XMLUtils::getXYZ(rotateElement);
    this->direction = rotateElement->BoolAttribute("clockwise", false) ? -1.0f : 1.0f;

    this->update(glfwGetTime());
}

void AnimatedRotation::update(float time) {
    this->rotationAngle = this->direction * time * glm::two_pi<float>() / this->rotationTime;
    this->matrix = glm::rotate(this->rotationAngle, this->rotationAxis);
}

}
