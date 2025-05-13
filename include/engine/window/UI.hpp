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

#include "engine/scene/camera/Camera.hpp"
#include "engine/window/FPSCounter.hpp"
#include "engine/window/Window.hpp"

namespace engine::window {

class UI {
private:
    scene::camera::Camera &camera;
    FPSCounter fpsCounter;
    int entityCount;
    const int &pickedId;
    bool fillPolygons, backFaceCulling, showAxes, showBoundingSpheres, showAnimationLines,
        showNormals;

public:
    UI(Window &window, scene::camera::Camera &_camera, int _entityCount, const int &pickedId);
    ~UI();

    bool isCapturingKeyboard() const;
    void draw(int renderedEntities);

    bool shouldFillPolygons() const;
    bool shouldCullBackFaces() const;
    bool shouldShowAxes() const;
    bool shouldShowBoundingSpheres() const;
    bool shouldShowAnimationLines() const;
    bool shouldShowNormals() const;
};

}
