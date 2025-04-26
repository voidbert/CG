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
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "engine/render/BoundingSphere.hpp"
#include "engine/render/RenderPipelineManager.hpp"

namespace engine::scene::camera {

class Camera {
protected:
    glm::vec3 position, lookAt, up;
    float fov, near, far, aspectRatio;

    glm::mat4 cameraMatrix;
    std::array<glm::vec4, 6> viewFrustum;

public:
    Camera(const glm::vec3 &_position,
           const glm::vec3 &_lookAt,
           const glm::vec3 &_up,
           float _fov,
           float _near,
           float _far);

    const glm::vec3 &getPosition() const;
    const glm::mat4 &getCameraMatrix() const;

    virtual void setPosition(const glm::vec3 &pos);
    void setWindowSize(int width, int height);

    virtual void move(const glm::vec3 &v);
    virtual void pan(const glm::vec2 &v);
    virtual void zoom(float factor);

    virtual int getEntityCount() const;
    virtual int draw(render::RenderPipelineManager &pipelineManager,
                     bool fillPolygons,
                     bool showBoundingSpheres) const;

    bool isInFrustum(const render::BoundingSphere &sphere) const;

protected:
    virtual void update();

private:
    static glm::vec4 planeEquation(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3);
};

}
