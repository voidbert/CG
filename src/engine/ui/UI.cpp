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

void UI::setup(Window *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window->getHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
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
    if (ImGui::Checkbox("Wireframe", &wireframe)) {
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }

    static bool culling = true;
    if (ImGui::Checkbox("Culling", &culling)) {
        if (culling) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
    }

    if (ImGui::Checkbox("Axes", &showAxes)) {}

    ImGui::Separator();
    ImGui::Text("Camera Controls:");

    if (this->camera) {
        if (firstCameraSync) {
            currentCamPos = this->camera->getPosition();
            firstCameraSync = false;
        }

        if (ImGui::InputFloat3("Position",
                               glm::value_ptr(currentCamPos),
                               "%.2f",
                               ImGuiInputTextFlags_EnterReturnsTrue)) {
            if (updateCameraPosition) {
                updateCameraPosition(currentCamPos);
            }
        }
    } else {
        ImGui::Text("No Camera Detected");
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::setCamera(camera::Camera *cam) {
    this->camera = cam;
}

void UI::setCameraPosition(const glm::vec3 &newPos) {
    this->currentCamPos = newPos;
}

void UI::setShowAxes(bool value) {
    this->showAxes = value;
}

void UI::setCameraUpdateCallbacks(std::function<void(const glm::vec3 &)> positionCallback,
                                  std::function<void(const std::string &)> typeCallback) {
    this->updateCameraPosition = positionCallback;
    this->updateCameraType = typeCallback;
}

bool UI::isShowAxesEnabled() const {
    return this->showAxes;
}

}
