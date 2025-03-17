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

#include "engine/ui/UI.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "engine/camera/Camera.hpp"
#include "engine/render/RenderPipeline.hpp"
#include "engine/window/Window.hpp"

namespace engine::ui {

UI::UI(Window *window, camera::Camera &_camera) : camera(_camera) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window->getHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

void UI::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Menu");

    static float lastTime = 0.0f;
    static int fps = 0;
    float currentTime = glfwGetTime();
    if (currentTime - lastTime >= 1.0f) {
        fps = static_cast<int>(1.0f / ImGui::GetIO().DeltaTime);
        lastTime = currentTime;
    }
    ImGui::Text("FPS: %d", fps);

    ImGui::Separator();
    ImGui::Text("Render Options");

    static bool wireframe = true;
    if (ImGui::Checkbox("Fill Polygons", &wireframe)) {
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }

    static bool culling = true;
    if (ImGui::Checkbox("Back-face Culling", &culling)) {
        if (culling) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
    }

    if (ImGui::Checkbox("Show Axes", &showAxes)) {}

    ImGui::Separator();
    ImGui::Text("Camera Options");

    glm::vec3 currentPos = camera.getPosition();
    if (ImGui::InputFloat3("Position", glm::value_ptr(currentPos), "%.2f")) {
        camera.setPosition(currentPos);
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::setShowAxes(bool value) {
    this->showAxes = value;
}

bool UI::isShowAxesEnabled() const {
    return this->showAxes;
}

}
