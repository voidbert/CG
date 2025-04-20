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

#include "engine/render/RenderPipelineManager.hpp"
#include "engine/scene/camera/CameraController.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/window/UI.hpp"
#include "engine/window/Window.hpp"

namespace engine::window {

class SceneWindow : public Window {
private:
    render::RenderPipelineManager pipelineManager;
    scene::Scene scene;
    scene::camera::CameraController cameraController;

    UI ui;
    bool showUI;

public:
    explicit SceneWindow(const std::string &sceneFile);
    SceneWindow(const SceneWindow &window) = delete;
    SceneWindow(SceneWindow &&window) = delete;

protected:
    void onUpdate(float time, float timeElapsed) override;
    void onRender() override;
    void onResize(int _width, int _height) override;
    void onKeyEvent(int key, int action) override;
};

}
