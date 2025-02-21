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

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "engine/SceneWindow.hpp"

namespace engine {

SceneWindow::SceneWindow(const std::string &sceneFile) :
    Window(sceneFile, 640, 480), pipeline(), scene(sceneFile) {
    this->resize(scene.getWindowWidth(), scene.getWindowHeight());

    // Only do this once, as we have a single shader program
    this->pipeline.use();

    // TODO - remove when all of the figures are using the right-hand rule
    // glFrontFace(GL_CCW);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void SceneWindow::onUpdate(float time, float timeElapsed) {
    GLFWwindow *const windowHandle = this->getHandle();

    const int up = glfwGetKey(windowHandle, GLFW_KEY_W);
    const int down = glfwGetKey(windowHandle, GLFW_KEY_S);
    const int left = glfwGetKey(windowHandle, GLFW_KEY_A);
    const int right = glfwGetKey(windowHandle, GLFW_KEY_D);

    const glm::vec3 direction((right - left), 0.0f, (down - up));
    this->scene.getCamera().move(direction, timeElapsed);
}

void SceneWindow::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    glm::mat4 cameraMatrix =
        scene.getCamera().getCameraMatrix(static_cast<float>(this->getWidth()) / this->getHeight());

    this->pipeline.setCameraMatrix(cameraMatrix);
    this->scene.draw(this->pipeline);
}

void SceneWindow::onResize(int _width, int _height) {
    glViewport(0, 0, _width, _height);
    scene.setWindowSize(_width, _height);
}

}
