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
#include <engine/render/RenderPipeline.hpp>
#include <engine/scene/Scene.hpp>
#include <engine/window/Window.hpp>
#include <functional>

namespace engine::ui {
class UI {
private:
    camera::Camera *camera = nullptr;
    bool showAxes = true;
    bool firstCameraSync = true;
    glm::vec3 currentCamPos;
    std::function<void(const glm::vec3 &)> updateCameraPosition;

public:
    void setup(Window *window);
    void render();
    void shutdown();

    void setCamera(camera::Camera *cam);
    void setCameraUpdateCallbacks(std::function<void(const glm::vec3 &)> positionCallback);
    void setCameraPosition(const glm::vec3 &newPos);
    void setShowAxes(bool value);

    bool isShowAxesEnabled() const;
};

}
