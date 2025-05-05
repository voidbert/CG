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

#include <memory>
#include <string>
#include <vector>

#include "engine/render/Axis.hpp"
#include "engine/render/RenderPipelineManager.hpp"
#include "engine/scene/camera/Camera.hpp"
#include "engine/scene/Group.hpp"
#include "engine/scene/light/Light.hpp"

namespace engine::scene {

class Scene {
private:
    int windowWidth, windowHeight;
    std::unique_ptr<camera::Camera> camera;
    std::vector<std::unique_ptr<Group>> groups;
    render::Axis xAxis, yAxis, zAxis;
    std::unique_ptr<light::Light> light;

public:
    explicit Scene(const std::string &file);
    Scene(const Scene &scene) = delete;
    Scene(Scene &&scene) = delete;

    int getWindowWidth() const;
    int getWindowHeight() const;
    int getEntityCount() const;
    camera::Camera &getCamera();

    void setWindowSize(int width, int height);

    void update(float time);

    int draw(render::RenderPipelineManager &pipelineManager,
             bool fillPolygons,
             bool backFaceCulling,
             bool showAxes,
             bool showBoundingSpheres,
             bool showAnimationLines) const;
};

}
