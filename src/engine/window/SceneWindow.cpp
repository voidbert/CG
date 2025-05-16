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

#include <array>
#include <cstdint>

#include "engine/render/Framebuffer.hpp"
#include "engine/window/SceneWindow.hpp"

namespace engine::window {

SceneWindow::SceneWindow(const std::string &sceneFile) :
    Window(sceneFile + " (press U to toggle UI)", 640, 480),
    scene(sceneFile),
    pipelineManager(scene.getPointLightCount(),
                    scene.getDirectionalLightCount(),
                    scene.getSpotlightCount()),
    cameraController(scene.getCamera()),
    ui(*this, scene.getCamera(), scene.getEntityCount()),
    selectedEntity(),
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
        this->ui.draw(renderedEntities, selectedEntity);
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

void SceneWindow::onMouseButtonEvent(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Draw scene to framebuffer
        render::Framebuffer framebuffer(this->getWidth(), this->getHeight());
        framebuffer.use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        std::unordered_map<int, std::string> idToName;
        this->scene.drawForPicking(this->pipelineManager, idToName);
        // Sample pixel color and determine ID
        double x, y;
        glfwGetCursorPos(this->getHandle(), &x, &y);
        const std::array<uint8_t, 3> pixelColor = framebuffer.sample(x, y);
        const int id = pixelColor[0] + (pixelColor[1] << 8) + (pixelColor[2] << 16);
        this->selectedEntity = idToName[id];

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, this->getWidth(), this->getHeight());
    }
}

}
