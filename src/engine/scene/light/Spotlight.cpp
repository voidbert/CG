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

#include "engine/scene/light/Spotlight.hpp"
#include <glm/geometric.hpp>

namespace engine::scene::light {

Spotlight::Spotlight(const glm::vec3 &_position, const glm::vec3 &_direction, float _cutoff) :
    position(_position), direction(glm::normalize(_direction)), cutoff(_cutoff) {}

const glm::vec3 &Spotlight::getPosition() const {
    return this->position;
}

const glm::vec3 &Spotlight::getDirection() const {
    return this->direction;
}

float Spotlight::getCutoff() const {
    return this->cutoff;
}

}
