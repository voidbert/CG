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

#include <glm/geometric.hpp>
#include <glm/gtx/transform.hpp>
#include <numeric>

#include "engine/render/BoundingSphere.hpp"
#include "engine/render/Model.hpp"
#include "generator/figures/Sphere.hpp"

namespace engine::render {

BoundingSphere::BoundingSphere() : center(0.0f, 0.0f, 0.0f, 1.0f), radius(0.0f) {
    if (!BoundingSphere::sphereModel && !BoundingSphere::initializingSphereModel) {
        BoundingSphere::initializingSphereModel = true;

        // This is only freed when the OpenGL context is destroyed
        generator::figures::Sphere sphere(1.0f, 16, 16);
        BoundingSphere::sphereModel = new Model(sphere);
    }
}

BoundingSphere::BoundingSphere(const glm::vec4 &_center, float _radius) : BoundingSphere() {
    this->center = _center;
    this->radius = _radius;
}

BoundingSphere::BoundingSphere(const BoundingSphere &sphere, const glm::mat4 &transform) {
    const float scalex = glm::length(transform[0]);
    const float scaley = glm::length(transform[1]);
    const float scalez = glm::length(transform[2]);

    this->radius = sphere.radius * std::max(scalex, std::max(scaley, scalez));
    this->center = transform * sphere.center;
}

BoundingSphere::BoundingSphere(const std::vector<glm::vec4> &vertices) : BoundingSphere() {
    // Calculate center of mass
    this->center = std::reduce(vertices.cbegin(), vertices.cend(), glm::vec4(0.0f), std::plus<>()) /
        static_cast<float>(vertices.size());

    // Calculate radius
    this->radius = std::transform_reduce(
        vertices.cbegin(),
        vertices.cend(),
        -1.0,
        [](float d1, float d2) { return std::max(d1, d2); },
        [this](const glm::vec4 &vertex) { return glm::distance(this->center, vertex); });
}

const glm::vec4 &BoundingSphere::getCenter() const {
    return this->center;
}

float BoundingSphere::getRadius() const {
    return this->radius;
}

void BoundingSphere::draw(RenderPipelineManager &pipelineManager,
                          const glm::mat4 &cameraMatrix,
                          const glm::vec4 &color) const {

    const glm::mat4 transformMatrix = cameraMatrix * glm::translate(glm::vec3(this->center)) *
        glm::scale(glm::vec3(this->radius));

    BoundingSphere::sphereModel->drawSolidColor(pipelineManager, transformMatrix, color, false);
}

Model *BoundingSphere::sphereModel = nullptr;
bool BoundingSphere::initializingSphereModel = false;

}
