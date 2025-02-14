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

#include "engine/SceneWindow.hpp"

namespace engine {
SceneWindow::SceneWindow() : Window("CG 2024/25", 640, 480), pipeline() {
    // Only do this once, as we have a single shader program
    this->pipeline.use();

    std::vector<glm::vec4> vertices { glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
                                      glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
                                      glm::vec4(0.0f, 0.5f, 0.0f, 1.0f) };
    std::vector<std::vector<int>> faces {
        { 0, 1, 2 }
    };
    this->model = std::make_unique<Model>(vertices, faces);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void SceneWindow::onUpdate(float time, float timeElapsed) {}

void SceneWindow::onRender() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    this->model->draw();
}

void SceneWindow::onResize(int _width, int _height) {
    glViewport(0, 0, _width, _height);
}

}
