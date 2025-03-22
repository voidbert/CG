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

#include <execution>
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

        generator::figures::Sphere sphere(1.0f, 16, 16);
        BoundingSphere::sphereModel = std::make_shared<Model>(sphere);
    }
}

BoundingSphere::BoundingSphere(const glm::vec4 &_center, float _radius) : BoundingSphere() {
    this->center = _center;
    this->radius = _radius;
}

BoundingSphere::BoundingSphere(const std::vector<utils::Vertex> &vertices) : BoundingSphere() {
    // Calculate center of mass
    this->center =
        std::transform_reduce(std::execution::par,
                              vertices.cbegin(),
                              vertices.cend(),
                              glm::vec4(0.0f),
                              std::plus<>(),
                              [](const utils::Vertex &vertex) { return vertex.position; }) /
        static_cast<float>(vertices.size());

    // Calculate radius
    this->radius = std::transform_reduce(
        std::execution::par,
        vertices.cbegin(),
        vertices.cend(),
        -1.0,
        [](float d1, float d2) { return std::max(d1, d2); },
        [this](const utils::Vertex &vertex) {
            return glm::distance(this->center, vertex.position);
        });
}

BoundingSphere::BoundingSphere(const BoundingSphere &sphere, const glm::mat4 &transform) {
    // https://math.stackexchange.com/questions/237369
    const float scalex = glm::length(transform[0]);
    const float scaley = glm::length(transform[1]);
    const float scalez = glm::length(transform[2]);

    this->radius = sphere.radius * std::max(scalex, std::max(scaley, scalez));
    this->center = transform * sphere.center;
}

glm::vec4 BoundingSphere::getCenter() const {
    return this->center;
}

float BoundingSphere::getRadius() const {
    return this->radius;
}

void BoundingSphere::draw(const RenderPipeline &pipeline, const glm::mat4 &cameraMatrix) const {
    const glm::vec3 translationVector(this->center);
    const glm::vec3 scaleVector(this->radius);
    pipeline.setMatrix(cameraMatrix * glm::translate(translationVector) * glm::scale(scaleVector));

    pipeline.setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    BoundingSphere::sphereModel->draw();
}

std::shared_ptr<Model> BoundingSphere::sphereModel = nullptr;
bool BoundingSphere::initializingSphereModel = false;

}
