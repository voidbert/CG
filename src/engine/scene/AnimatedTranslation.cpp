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
#include <stdexcept>

#include "engine/scene/AnimatedTranslation.hpp"
#include "utils/XMLUtils.hpp"
#include <GLFW/glfw3.h>

namespace engine::scene {

AnimatedTranslation::AnimatedTranslation(const tinyxml2::XMLElement *translateElement) {
    this->time = translateElement->FloatAttribute("time", NAN);
    if (std::isnan(this->time) || this->time <=0) {
        throw std::runtime_error("Invalid translation time <translate> in scene XML file");
    }
    this->align = translateElement->BoolAttribute("align",false);
    const tinyxml2::XMLElement* point = translateElement->FirstChildElement("point");
    int pointCount = 0;
    while (point != nullptr) {
        glm::vec3 p = utils::XMLUtils::getXYZ(point);
        this->points.push_back(p);
        pointCount++;
        point = point->NextSiblingElement("point");
    }
    if (pointCount < 4) {
        throw std::runtime_error("Invalid number of translation points <translate> in scene XML file");
    }
}

glm::vec3 CatmullRom(const std::vector<glm::vec3> &points, float t) {
    int p = (int)t;
    float f = t - p;

    glm::vec3 p0 = points[(p - 1 + points.size()) % points.size()];
    glm::vec3 p1 = points[p % points.size()];
    glm::vec3 p2 = points[(p + 1) % points.size()];
    glm::vec3 p3 = points[(p + 2) % points.size()];

    return 0.5f * ((-p0 + 3.0f * p1 - 3.0f * p2 + p3) * (f * f * f) +
                   (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * (f * f) +
                   (-p0 + p2) * f +
                   2.0f * p1);
}

glm::mat4 AnimatedTranslation::getMatrix() const {
    float elapsedTime = glfwGetTime();
    float normalizedTime = fmod(elapsedTime, time) / time;
    float curveT = normalizedTime * (points.size());
    glm::vec3 position = CatmullRom(points, curveT);
    return glm::translate(position);
}

}
