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

#include "engine/render/BoundingSphere.hpp"
#include "engine/render/Model.hpp"
#include "generator/figures/Sphere.hpp"

namespace engine::render {

BoundingSphere::BoundingSphere() {
    if (!BoundingSphere::sphereModel && !BoundingSphere::initializingSphereModel) {
        BoundingSphere::initializingSphereModel = true;

        generator::figures::Sphere sphere(1.0f, 16, 16);
        BoundingSphere::sphereModel = std::make_shared<Model>(sphere);
    }
}

BoundingSphere::BoundingSphere(glm::vec4 _center, float _radius) : BoundingSphere() {
    this->center = _center;
    this->radius = _radius;
}

BoundingSphere::BoundingSphere(const std::vector<utils::Vertex> &vertices) : BoundingSphere() {
    glm::vec4 sum(0.0f);
    for (const utils::Vertex &vertex : vertices) {
        sum += vertex.position;
    }
    this->center = sum / static_cast<float>(vertices.size());

    float maxDistance = -1.0f;
    for (const utils::Vertex &vertex : vertices) {
        float distance = glm::distance(this->center, vertex.position);
        if (distance > maxDistance) {
            maxDistance = distance;
        }
    }
    this->radius = maxDistance;
}

glm::vec4 BoundingSphere::getCenter() const {
    return this->center;
}

float BoundingSphere::getRadius() const {
    return this->radius;
}

void BoundingSphere::draw(const RenderPipeline &pipeline, const glm::mat4 &transform) const {
    const glm::vec3 translationVector = glm::vec3(this->center.x, this->center.y, this->center.z);
    const glm::vec3 scaleVector = glm::vec3(this->radius);
    pipeline.setMatrix(transform * glm::translate(translationVector) * glm::scale(scaleVector));

    pipeline.setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    BoundingSphere::sphereModel->draw();
}

std::shared_ptr<Model> BoundingSphere::sphereModel = nullptr;
bool BoundingSphere::initializingSphereModel = false;

}
