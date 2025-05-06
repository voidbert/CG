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
#include <stdexcept>

#include "utils/XMLUtils.hpp"

namespace utils {

const tinyxml2::XMLElement *XMLUtils::getSingleChild(const tinyxml2::XMLNode *parent,
                                                     const std::string &name) {

    const tinyxml2::XMLElement *child = parent->FirstChildElement(name.c_str());
    if (!child) {
        throw std::runtime_error("<" + name + "> element not found in scene XML");
    }

    const tinyxml2::XMLElement *child2 = child->NextSiblingElement(name.c_str());
    if (child2) {
        throw std::runtime_error("More than one <" + name + "> element in scene XML");
    }

    return child;
}

glm::vec3 XMLUtils::getXYZ(const tinyxml2::XMLElement *element) {
    float x = element->FloatAttribute("x", NAN);
    float y = element->FloatAttribute("y", NAN);
    float z = element->FloatAttribute("z", NAN);

    if (std::isnan(x) || std::isnan(y) || std::isnan(z)) {
        std::string name = element->Name();
        throw std::runtime_error("Invalid vector in <" + name + "> in scene XML file");
    }

    return glm::vec3(x, y, z);
}

glm::vec3 XMLUtils::getRGB(const tinyxml2::XMLElement *element) {
    float R = element->FloatAttribute("R", NAN);
    float G = element->FloatAttribute("G", NAN);
    float B = element->FloatAttribute("B", NAN);

    if (std::isnan(R) || std::isnan(G) || std::isnan(B)) {
        std::string name = element->Name();
        throw std::runtime_error("Invalid vector in <" + name + "> in scene XML file");
    }

    return glm::vec3(R, G, B);
}

}
