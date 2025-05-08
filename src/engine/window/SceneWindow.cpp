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

#include "engine/window/SceneWindow.hpp"

namespace engine::window {

SceneWindow::SceneWindow(const std::string &sceneFile) :
    Window(sceneFile + " (press U to toggle UI)", 640, 480),
    pipelineManager(),
    scene(sceneFile),
    cameraController(scene.getCamera()),
    ui(*this, scene.getCamera(), scene.getEntityCount()),
    showUI(true) {

    glEnable(GL_DEPTH_TEST);
    this->resize(scene.getWindowWidth(), scene.getWindowHeight());
}

void SceneWindow::onUpdate(float time, float timeElapsed) {
    static_cast<void>(timeElapsed);
    this->cameraController.onUpdate(time);
    this->scene.update(time);
}

void SceneWindow::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    const int renderedEntities = this->scene.draw(this->pipelineManager,
                                                  this->ui.shouldFillPolygons(),
                                                  this->ui.shouldCullBackFaces(),
                                                  this->ui.shouldShowAxes(),
                                                  this->ui.shouldShowBoundingSpheres(),
                                                  this->ui.shouldShowAnimationLines(),
                                                  this->ui.shouldShowNormals());

    if (this->showUI) {
        this->ui.draw(renderedEntities);
    }
}

void SceneWindow::onResize(int _width, int _height) {
    glViewport(0, 0, _width, _height);
    scene.setWindowSize(_width, _height);
}

void SceneWindow::onKeyEvent(int key, int action) {
    if (!this->ui.isCapturingKeyboard()) {
        this->cameraController.onKeyEvent(key, action);
    }

    if (key == GLFW_KEY_U && action == GLFW_PRESS) {
        this->showUI = !this->showUI;
    }
}

}
