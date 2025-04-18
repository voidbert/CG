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

#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "engine/render/BoundingSphere.hpp"
#include "engine/render/RenderPipeline.hpp"

namespace engine::camera {

enum class MovementDirection {
    Up,
    Down,
    Left,
    Right,
    Forward,
    Backward,
    LookLeft,
    LookRight,
    LookUp,
    LookDown
};

class Camera {
protected:
    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 up;
    float fov;
    float near;
    float far;
    float aspectRatio;

    glm::mat4 cameraMatrix;
    std::array<glm::vec4, 6> viewFrustum;

public:
    Camera(const glm::vec3 &_position,
           const glm::vec3 &_lookAt,
           const glm::vec3 &_up,
           float _fov,
           float _near,
           float _far);

    virtual void setPosition(const glm::vec3 &pos);
    void setWindowSize(int width, int height);
    virtual void move(MovementDirection direction, float deltaTime) = 0;

    const glm::vec3 &getPosition() const;
    const glm::mat4 &getCameraMatrix() const;

    bool isInFrustum(const render::BoundingSphere &sphere) const;

    virtual void draw(const render::RenderPipeline &pipeline, bool drawBoundingSpheres);

protected:
    void updateCameraMatrix();
    void updateViewFrustum();

private:
    static glm::vec4 planeEquation(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3);
};

}
