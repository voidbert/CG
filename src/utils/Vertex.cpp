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

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>

#include "utils/Vertex.hpp"

namespace utils {

Vertex::Vertex(const glm::vec4 &_position) : position(_position) {}
Vertex::Vertex(float x, float y, float z) : position(x, y, z, 1.0f) {}

}

namespace std {

size_t hash<utils::Vertex>::operator()(const utils::Vertex &p) const {
    return hash<glm::vec4> {}(p.position);
}

template struct std::hash<utils::Vertex>;

}
