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
#include <cmath>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>

#include "engine/scene/camera/Camera.hpp"

namespace engine::scene::camera {

Camera::Camera(const glm::vec3 &_position,
               const glm::vec3 &_lookAt,
               const glm::vec3 &_up,
               float _fov,
               float _near,
               float _far) :
    position(_position),
    lookAt(_lookAt),
    up(_up),
    fov(_fov),
    near(_near),
    far(_far),
    aspectRatio(1.0f) {

    this->updateWithMotion();
}

const glm::vec3 &Camera::getPosition() const {
    return this->position;
}

const glm::mat4 &Camera::getCameraMatrix() const {
    return this->cameraMatrix;
}

void Camera::setPosition(const glm::vec3 &pos) {
    this->position = pos;
    this->updateWithMotion();
}

void Camera::setWindowSize(int width, int height) {
    this->aspectRatio = static_cast<float>(width) / height;
    this->updateWithMotion();
}

void Camera::move(const glm::vec3 &v) {
    static_cast<void>(v);
}

void Camera::pan(const glm::vec2 &v) {
    static_cast<void>(v);
}

void Camera::zoom(float factor) {
    static_cast<void>(factor);
}

int Camera::getEntityCount() const {
    return 0;
}

void Camera::updateWithTime(float time) {
    static_cast<void>(time);
}

void Camera::drawSolidColorParts(render::RenderPipelineManager &pipelineManager,
                                 bool showBoundingSpheres,
                                 bool showAnimationLines,
                                 bool showNormals) const {

    static_cast<void>(pipelineManager);
    static_cast<void>(showBoundingSpheres);
    static_cast<void>(showAnimationLines);
    static_cast<void>(showNormals);
}

int Camera::drawShadedParts(render::RenderPipelineManager &pipelineManager,
                            bool fillPolygons) const {

    static_cast<void>(pipelineManager);
    static_cast<void>(fillPolygons);
    return 0;
}

int Camera::drawForPicking(render::RenderPipelineManager &pipelineManager, int currentId) const {
    static_cast<void>(pipelineManager);
    static_cast<void>(currentId);
    return currentId;
}

bool Camera::isInFrustum(const render::BoundingSphere &sphere) const {
    return std::none_of(this->viewFrustum.cbegin(),
                        this->viewFrustum.cend(),
                        [sphere](const glm::vec4 &plane) {
                            const float distance =
                                glm::dot(glm::vec3(plane), glm::vec3(sphere.getCenter())) + plane.w;
                            return distance < -sphere.getRadius();
                        });
}

void Camera::updateWithMotion() {
    // Update camera matrix
    const glm::mat4 view = glm::lookAt(this->position, this->lookAt, this->up);
    const glm::mat4 projection =
        glm::perspective(this->fov, this->aspectRatio, this->near, this->far);

    this->cameraMatrix = projection * view;

    // Update view frustum
    const float halfNearHeight = tanf(this->fov / 2) * this->near;
    const float halfNearWidth = halfNearHeight * this->aspectRatio;

    const float halfFarHeight = tanf(this->fov / 2) * this->far;
    const float halfFarWidth = halfFarHeight * this->aspectRatio;

    const glm::vec3 d = glm::normalize(this->lookAt - this->position);
    const glm::vec3 right = glm::normalize(glm::cross(d, this->up));
    const glm::vec3 realUp = glm::normalize(glm::cross(right, d));

    const glm::vec3 farCenter = this->position + d * this->far;
    const glm::vec3 nearCenter = this->position + d * this->near;

    const glm::vec3 nearTopLeft = nearCenter + realUp * halfNearHeight - right * halfNearWidth;
    const glm::vec3 nearTopRight = nearCenter + realUp * halfNearHeight + right * halfNearWidth;
    const glm::vec3 nearBottomLeft = nearCenter - realUp * halfNearHeight - right * halfNearWidth;
    const glm::vec3 nearBottomRight = nearCenter - realUp * halfNearHeight + right * halfNearWidth;

    const glm::vec3 farTopLeft = farCenter + realUp * halfFarHeight - right * halfFarWidth;
    const glm::vec3 farTopRight = farCenter + realUp * halfFarHeight + right * halfFarWidth;
    const glm::vec3 farBottomLeft = farCenter - realUp * halfFarHeight - right * halfFarWidth;
    const glm::vec3 farBottomRight = farCenter - realUp * halfFarHeight + right * halfFarWidth;

    this->viewFrustum[0] = Camera::planeEquation(nearTopLeft, nearTopRight, nearBottomLeft);
    this->viewFrustum[1] = Camera::planeEquation(farTopLeft, farBottomLeft, farTopRight);
    this->viewFrustum[2] = Camera::planeEquation(nearTopLeft, nearBottomLeft, farTopLeft);
    this->viewFrustum[3] = Camera::planeEquation(nearTopRight, farTopRight, nearBottomRight);
    this->viewFrustum[4] = Camera::planeEquation(nearTopLeft, farTopLeft, nearTopRight);
    this->viewFrustum[5] = Camera::planeEquation(farBottomRight, farBottomLeft, nearBottomLeft);
}

glm::vec4 Camera::planeEquation(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3) {
    const glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
    const float constantFactor = glm::dot(-normal, p1);
    return glm::vec4(normal, constantFactor);
}

}
