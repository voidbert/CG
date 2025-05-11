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

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>

#include "engine/scene/camera/OrbitalCamera.hpp"
#include "engine/scene/Group.hpp"

namespace engine::scene::camera {

class ThirdPersonCamera : public OrbitalCamera {
private:
    glm::mat4 playerTransform;
    std::unique_ptr<scene::Group> player;

public:
    ThirdPersonCamera(const glm::vec3 &_position,
                      const glm::vec3 &_lookAt,
                      const glm::vec3 &_up,
                      float _fov,
                      float _near,
                      float _far,
                      std::unique_ptr<scene::Group> _player);

    void move(const glm::vec3 &v) override;

    int getEntityCount() const override;

    virtual void updateWithTime(float time) override;

    virtual void drawSolidColorParts(render::RenderPipelineManager &pipelineManager,
                                     bool showBoundingSpheres,
                                     bool showAnimationLines,
                                     bool showNormals) const override;
    virtual int drawShadedParts(render::RenderPipelineManager &pipelineManager,
                                bool fillPolygons) const override;

protected:
    virtual void updateWithMotion() override;
};

}
