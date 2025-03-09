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

#include <glm/gtx/transform.hpp>

#include "engine/scene/Translation.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::scene {

Translation::Translation(const tinyxml2::XMLElement *translateElement) :
    translateVector(utils::XMLUtils::getXYZ(translateElement)) {}

glm::mat4 Translation::getMatrix() const {
    return glm::translate(this->translateVector);
}

}
