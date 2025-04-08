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

#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <stdexcept>

#include "engine/scene/AnimatedTranslation.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::scene {

AnimatedTranslation::AnimatedTranslation(const tinyxml2::XMLElement *translateElement) {
    this->time = translateElement->FloatAttribute("time", NAN);
    if (std::isnan(this->time) || this->time <= 0) {
        throw std::runtime_error("Invalid translation time <translate> in scene XML file");
    }
    this->align = translateElement->BoolAttribute("align", false);
    const tinyxml2::XMLElement *point = translateElement->FirstChildElement("point");
    int pointCount = 0;
    while (point != nullptr) {
        glm::vec3 p = utils::XMLUtils::getXYZ(point);
        this->points.push_back(p);
        pointCount++;
        point = point->NextSiblingElement("point");
    }
    if (pointCount < 4) {
        throw std::runtime_error(
            "Invalid number of translation points <translate> in scene XML file");
    }
}

namespace {

void getCatmullRomPoint(float t,
                        const glm::vec3 &p0,
                        const glm::vec3 &p1,
                        const glm::vec3 &p2,
                        const glm::vec3 &p3,
                        glm::vec3 &pos,
                        glm::vec3 &deriv) {

    float t2 = t * t;
    float t3 = t2 * t;

    // Vetor T = [t^3, t^2, t, 1]
    glm::vec4 T(t3, t2, t, 1.0f);
    glm::vec4 dT(3 * t2, 2 * t, 1.0f, 0.0f);

    // Matriz M (Catmull-Rom)
    glm::mat4 M = 0.5f * glm::mat4(-1, 2, -1, 0, 3, -5, 0, 2, -3, 4, 1, 0, 1, -1, 0, 0);

    // Vetores com as componentes x, y e z dos pontos de controle
    glm::vec4 px(p0.x, p1.x, p2.x, p3.x);
    glm::vec4 py(p0.y, p1.y, p2.y, p3.y);
    glm::vec4 pz(p0.z, p1.z, p2.z, p3.z);

    // Multiplicações para posição
    float x = glm::dot(T, M * px);
    float y = glm::dot(T, M * py);
    float z = glm::dot(T, M * pz);
    pos = glm::vec3(x, y, z);

    // Multiplicações para derivada
    float dx = glm::dot(dT, M * px);
    float dy = glm::dot(dT, M * py);
    float dz = glm::dot(dT, M * pz);
    deriv = glm::vec3(dx, dy, dz);
}

void getGlobalCatmullRomPoint(float globalT,
                              const std::vector<glm::vec3> &points,
                              glm::vec3 &pos,
                              glm::vec3 &deriv) {
    int pointCount = points.size();
    float t = globalT;
    int segment = (int) floor(t);
    float localT = t - segment;

    glm::vec3 p0 = points[(segment - 1 + pointCount) % pointCount];
    glm::vec3 p1 = points[segment % pointCount];
    glm::vec3 p2 = points[(segment + 1) % pointCount];
    glm::vec3 p3 = points[(segment + 2) % pointCount];

    getCatmullRomPoint(localT, p0, p1, p2, p3, pos, deriv);
}
}

glm::mat4 AnimatedTranslation::getMatrix() const {
    float elapsedTime = glfwGetTime();
    float normalizedTime = fmod(elapsedTime, time) / time;
    float curveT = normalizedTime * points.size();

    glm::vec3 pos, deriv;
    getGlobalCatmullRomPoint(curveT, points, pos, deriv);

    return glm::translate(pos);
}

}
