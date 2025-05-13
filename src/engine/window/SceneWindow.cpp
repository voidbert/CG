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

#include <iostream>

#include "engine/window/SceneWindow.hpp"

namespace engine::window {

SceneWindow::SceneWindow(const std::string &sceneFile) :
    Window(sceneFile + " (press U to toggle UI)", 640, 480),
    scene(sceneFile),
    pipelineManager(scene.getPointLightCount(),
                    scene.getDirectionalLightCount(),
                    scene.getSpotlightCount()),
    cameraController(scene.getCamera()),
    ui(*this, scene.getCamera(), scene.getEntityCount(), this->pickedId),
    showUI(true),
    pickedId(0) {

    glEnable(GL_DEPTH_TEST);
    this->resize(scene.getWindowWidth(), scene.getWindowHeight());
    this->initPickingFramebuffer();
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

void SceneWindow::initPickingFramebuffer() {
    glGenFramebuffers(1, &this->pickingFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, this->pickingFBO);

    glGenTextures(1, &this->pickingTexture);
    glBindTexture(GL_TEXTURE_2D, this->pickingTexture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 this->getWidth(),
                 this->getHeight(),
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           this->pickingTexture,
                           0);

    glGenRenderbuffers(1, &this->pickingDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, this->pickingDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->getWidth(), this->getHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              this->pickingDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create picking framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int SceneWindow::readIdAtMousePos() const {
    double xpos, ypos;
    glfwGetCursorPos(this->getHandle(), &xpos, &ypos);

    glBindFramebuffer(GL_FRAMEBUFFER, this->pickingFBO);
    glFlush();
    glFinish();

    unsigned char data[3];
    glReadPixels(static_cast<int>(xpos),
                 this->getHeight() - static_cast<int>(ypos),
                 1,
                 1,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 data);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return data[0] + (data[1] << 8) + (data[2] << 16);
}

void SceneWindow::onMouseButtonEvent(int button, int action, int mods) {
    static_cast<void>(mods); // ignorar mods

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glBindFramebuffer(GL_FRAMEBUFFER, this->pickingFBO);
        glViewport(0, 0, this->getWidth(), this->getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->pipelineManager.getPickingShaderProgram().use();
        this->scene.drawPickingParts(this->pipelineManager, this->scene.getCamera());

        this->pickedId = this->readIdAtMousePos();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

int SceneWindow::getPickedId() const {
    return this->pickedId;
}

}
