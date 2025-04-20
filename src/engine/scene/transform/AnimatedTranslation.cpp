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

#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/gtx/transform.hpp>
#include <stdexcept>

#include "engine/scene/transform/AnimatedTranslation.hpp"
#include "utils/Vertex.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::scene::transform {

AnimatedTranslation::AnimatedTranslation(const tinyxml2::XMLElement *translateElement) :
    lastUp(0.0f, 1.0f, 0.0f) {

    // Parse XML
    this->translationTime = translateElement->FloatAttribute("time", NAN);
    if (std::isnan(this->translationTime)) {
        throw std::runtime_error("<translate> missing time attribute in scene XML file");
    }

    const tinyxml2::XMLElement *point = translateElement->FirstChildElement();
    for (; point; point = point->NextSiblingElement()) {
        const std::string name = point->Name();

        if (name == "point") {
            this->points.push_back(utils::XMLUtils::getXYZ(point));
        } else {
            throw std::runtime_error("Invalid <" + name + "> in <translate> in scene XML file");
        }
    }

    if (this->points.size() < 4) {
        throw std::runtime_error("Too few points in <translate> in scene XML file");
    }

    this->align = translateElement->BoolAttribute("align", false);
    this->update(glfwGetTime());

    // Create renderable line
    std::vector<utils::Vertex> lineVertices;

    const int pointsPerSegment = 8;
    const int totalPoints = (this->points.size() - 3) * pointsPerSegment;
    const float iterationIncrement = this->translationTime / totalPoints;

    lineVertices.reserve(totalPoints);
    for (int i = 0; i < totalPoints; ++i) {
        const glm::vec3 linePoint = this->interpolate(i * iterationIncrement).first;
        lineVertices.push_back(utils::Vertex(glm::vec4(linePoint, 1.0f)));
    }

    this->line = std::make_unique<render::LineLoop>(lineVertices);
}

void AnimatedTranslation::update(float time) {
    std::pair<glm::vec3, glm::vec3> positionDerivative = this->interpolate(time);

    if (align) {
        const glm::vec3 mx = glm::normalize(positionDerivative.second);
        const glm::vec3 mz = glm::normalize(glm::cross(this->lastUp, mx));
        const glm::vec3 my = glm::normalize(glm::cross(mx, mz));
        const glm::mat4 rotation = { glm::vec4(mx, 0.0f),
                                     glm::vec4(my, 0.0f),
                                     glm::vec4(mz, 0.0f),
                                     glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };

        this->matrix = glm::translate(positionDerivative.first) * rotation;
        this->lastUp = my;
    } else {
        this->matrix = glm::translate(positionDerivative.first);
    }
}

void AnimatedTranslation::draw(render::RenderPipelineManager &pipelineManager,
                               const glm::mat4 &transformMatrix) const {

    this->line->draw(pipelineManager, transformMatrix, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
}

std::pair<glm::vec3, glm::vec3> AnimatedTranslation::interpolate(float time) {
    // Get segment and position in it
    const float normalizedTime = (time / this->translationTime) * this->points.size();
    const int segment = floorf(normalizedTime);
    const float localT = normalizedTime - segment;

    const glm::vec3 &p0 = this->points[(segment - 1 + this->points.size()) % this->points.size()];
    const glm::vec3 &p1 = this->points[segment % this->points.size()];
    const glm::vec3 &p2 = this->points[(segment + 1) % this->points.size()];
    const glm::vec3 &p3 = this->points[(segment + 2) % this->points.size()];

    // Interpolate curve
    const glm::mat4 m = {
        { -0.5f, 1.0f,  -0.5f, 0.0f },
        { 1.5f,  -2.5f, 0.0f,  1.0f },
        { -1.5f, 2.0f,  0.5f,  0.0f },
        { 0.5f,  -0.5f, 0.0f,  0.0f }
    };

    const glm::vec4 px(p0.x, p1.x, p2.x, p3.x);
    const glm::vec4 py(p0.y, p1.y, p2.y, p3.y);
    const glm::vec4 pz(p0.z, p1.z, p2.z, p3.z);

    const glm::vec4 tvec(powf(localT, 3.0f), powf(localT, 2.0f), localT, 1.0f);
    const glm::vec4 dtvec(3.0f * powf(localT, 2.0f), 2.0f * localT, 1.0f, 0.0f);

    const glm::vec3 position =
        glm::vec3(glm::dot(tvec, m * px), glm::dot(tvec, m * py), glm::dot(tvec, m * pz));
    const glm::vec3 derivative =
        glm::vec3(glm::dot(dtvec, m * px), glm::dot(dtvec, m * py), glm::dot(dtvec, m * pz));

    return std::make_pair(position, derivative);
}

}
