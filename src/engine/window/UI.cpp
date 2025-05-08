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

#include <glm/gtc/type_ptr.hpp>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "engine/window/UI.hpp"

namespace engine::window {

UI::UI(Window &window, scene::camera::Camera &_camera, int _entityCount) :
    camera(_camera),
    fpsCounter(),
    entityCount(_entityCount),
    fillPolygons(false),
    backFaceCulling(true),
    showAxes(true),
    showBoundingSpheres(false),
    showAnimationLines(true),
    showNormals(false) {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool UI::isCapturingKeyboard() const {
    const ImGuiIO &io = ImGui::GetIO();
    return io.WantCaptureKeyboard || io.WantTextInput;
}

void UI::draw(int renderedEntities) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Menu");

    this->fpsCounter.countFrame();
    ImGui::Text("FPS: %d", this->fpsCounter.getFPS());

    const std::string entityText = std::to_string(renderedEntities) + " / " +
        std::to_string(this->entityCount) + " entities rendered";
    ImGui::Text(entityText.c_str());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Render Options");
    ImGui::Spacing();

    ImGui::Checkbox("Fill Polygons", &this->fillPolygons);
    ImGui::Checkbox("Back-face Culling", &this->backFaceCulling);
    ImGui::Checkbox("Show Axes", &this->showAxes);
    ImGui::Checkbox("Show Bounding Spheres", &this->showBoundingSpheres);
    ImGui::Checkbox("Show Animation Lines", &this->showAnimationLines);
    ImGui::Checkbox("Show Normals", &this->showNormals);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Camera Options");
    ImGui::Spacing();

    glm::vec3 cameraPosition = this->camera.getPosition();
    if (ImGui::InputFloat3("Position", glm::value_ptr(cameraPosition), "%.1f")) {
        camera.setPosition(cameraPosition);
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UI::shouldFillPolygons() const {
    return this->fillPolygons;
}

bool UI::shouldCullBackFaces() const {
    return this->backFaceCulling;
}

bool UI::shouldShowAxes() const {
    return this->showAxes;
}

bool UI::shouldShowBoundingSpheres() const {
    return this->showBoundingSpheres;
}

bool UI::shouldShowAnimationLines() const {
    return this->showAnimationLines;
}

bool UI::shouldShowNormals() const {
    return this->showNormals;
}

}
