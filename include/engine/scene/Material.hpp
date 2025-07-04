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

#pragma once

#include <glm/vec3.hpp>
#include <tinyxml2.h>

namespace engine::scene {

class Material {
private:
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
    glm::vec3 emissive;
    float shininess;

public:
    Material();
    explicit Material(const tinyxml2::XMLElement *colorElement);

    const glm::vec3 &getDiffuse() const;
    const glm::vec3 &getAmbient() const;
    const glm::vec3 &getSpecular() const;
    const glm::vec3 &getEmissive() const;
    float getShininess() const;
};

}
