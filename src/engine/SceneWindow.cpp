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

#include "engine/SceneWindow.hpp"

namespace engine {
SceneWindow::SceneWindow() : Window("CG 2024/25", 640, 480), pipeline(), translate() {
    // Only do this once, as we have a single shader program
    this->pipeline.use();

    std::vector<utils::Vertex> vertices1 { utils::Vertex(-0.5f, -0.5f, 0.0f),
                                           utils::Vertex(0.5f, -0.5f, 0.0f),
                                           utils::Vertex(0.0f, 0.5f, 0.0f) };

    std::vector<utils::Vertex> vertices2 { utils::Vertex(0.5f, 0.5f, 0.0f),
                                           utils::Vertex(-0.5f, 0.5f, 0.0f),
                                           utils::Vertex(0.0f, -0.5f, 0.0f) };
    std::vector<uint32_t> indices { 0, 1, 2 };

    this->model1 = std::make_unique<Model>(vertices1, indices);
    this->model2 = std::make_unique<Model>(vertices1, indices);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void SceneWindow::onUpdate(float time, float timeElapsed) {
    GLFWwindow *const windowHandle = this->getHandle();

    const int up = glfwGetKey(windowHandle, GLFW_KEY_W);
    const int down = glfwGetKey(windowHandle, GLFW_KEY_S);
    const int left = glfwGetKey(windowHandle, GLFW_KEY_A);
    const int right = glfwGetKey(windowHandle, GLFW_KEY_D);

    const glm::vec3 direction((right - left) * 0.01f, (up - down) * 0.01f, 0.0f);
    this->translate += direction;
}

void SceneWindow::onRender() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    this->pipeline.setCameraMatrix(glm::mat4(1.0f));
    this->pipeline.setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    this->model1->draw();

    this->pipeline.setCameraMatrix(glm::translate(glm::mat4(1.0f), this->translate));
    this->pipeline.setColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    this->model2->draw();
}

void SceneWindow::onResize(int _width, int _height) {
    glViewport(0, 0, _width, _height);
}

}
