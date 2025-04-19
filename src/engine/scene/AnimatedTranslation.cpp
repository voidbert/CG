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

#include "engine/render/Line.hpp"
#include "engine/scene/AnimatedTranslation.hpp"
#include "utils/Vertex.hpp"
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
    this->catmullRomMotionLine.update(getLinePoints());
}

void AnimatedTranslation::getCatmullRomPoint(float t,
                                             const glm::vec3 &p0,
                                             const glm::vec3 &p1,
                                             const glm::vec3 &p2,
                                             const glm::vec3 &p3,
                                             glm::vec3 &pos,
                                             glm::vec3 &deriv) const {

    float t2 = t * t;
    float t3 = t2 * t;

    glm::vec4 T(t3, t2, t, 1.0f);
    glm::vec4 dT(3 * t2, 2 * t, 1.0f, 0.0f);

    // Matriz M (Catmull-Rom)
    glm::mat4 M = glm::mat4(-0.5, 1, -0.5, 0, 1.5, -2.5, 0, 1, -1.5, 2, 0.5, 0, 0.5, -0.5, 0, 0);

    glm::vec4 px(p0.x, p1.x, p2.x, p3.x);
    glm::vec4 py(p0.y, p1.y, p2.y, p3.y);
    glm::vec4 pz(p0.z, p1.z, p2.z, p3.z);

    float x = glm::dot(T, M * px);
    float y = glm::dot(T, M * py);
    float z = glm::dot(T, M * pz);
    pos = glm::vec3(x, y, z);

    float dx = glm::dot(dT, M * px);
    float dy = glm::dot(dT, M * py);
    float dz = glm::dot(dT, M * pz);
    deriv = glm::vec3(dx, dy, dz);
}

void AnimatedTranslation::getGlobalCatmullRomPoint(float globalT,
                                                   const std::vector<glm::vec3> &ppoints,
                                                   glm::vec3 &pos,
                                                   glm::vec3 &deriv) const {
    int pointCount = ppoints.size();
    float t = globalT;
    int segment = (int) floor(t);
    float localT = t - segment;

    glm::vec3 p0 = ppoints[(segment - 1 + pointCount) % pointCount];
    glm::vec3 p1 = ppoints[segment % pointCount];
    glm::vec3 p2 = ppoints[(segment + 1) % pointCount];
    glm::vec3 p3 = ppoints[(segment + 2) % pointCount];

    getCatmullRomPoint(localT, p0, p1, p2, p3, pos, deriv);
}

glm::mat4 AnimatedTranslation::getMatrix() const {
    float elapsedTime = glfwGetTime();
    float normalizedTime = fmod(elapsedTime, time) / time;
    float curveT = normalizedTime * points.size();

    glm::vec3 pos, deriv;
    getGlobalCatmullRomPoint(curveT, points, pos, deriv);

    if (this->align == true) {
        glm::vec3 forward = glm::normalize(deriv);
        glm::vec3 right = glm::normalize(glm::cross(lastUp, forward));

        glm::vec3 newUp = glm::normalize(glm::cross(forward, right));

        lastUp = newUp;

        glm::mat4 rotation = glm::mat4(1.0f);
        rotation[0] = glm::vec4(right, 0.0f);
        rotation[1] = glm::vec4(newUp, 0.0f);
        rotation[2] = glm::vec4(forward, 0.0f);

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos);
        return translation * rotation;
    } else {
        return glm::translate(pos);
    }
}

std::vector<utils::Vertex> AnimatedTranslation::getLinePoints() const {
    std::vector<utils::Vertex> pointsLine;
    float tessellation = 0.01f;
    int numSegments = (int) (1.0f / tessellation);
    glm::vec3 pos, deriv;

    for (int i = 0; i <= numSegments; i++) {
        float t = (float) i / (float) numSegments;
        float globalT = t * points.size();
        getGlobalCatmullRomPoint(globalT, points, pos, deriv);
        utils::Vertex v = utils::Vertex(pos.x, pos.y, pos.z);
        pointsLine.push_back(v);
    }
    return pointsLine;
}

int AnimatedTranslation::draw(const render::RenderPipeline &pipeline,
                              const glm::mat4 &_transform) const {
    this->catmullRomMotionLine.draw(pipeline, _transform);
    return 0;
}

}
