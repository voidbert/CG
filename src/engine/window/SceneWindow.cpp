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
#include <map>

#include "engine/camera/Camera.hpp"
#include "engine/ui/UI.hpp"
#include "engine/camera/FreeCamera.hpp"
#include "engine/camera/OrbitalCamera.hpp"
#include "engine/window/SceneWindow.hpp"

namespace engine::window {

SceneWindow::SceneWindow(const std::string &sceneFile) :
    Window(sceneFile, 640, 480),
    pipeline(),
    scene(sceneFile),
    xAxis(glm::vec3(1.0f, 0.0f, 0.0f)),
    yAxis(glm::vec3(0.0f, 1.0f, 0.0f)),
    zAxis(glm::vec3(0.0f, 0.0f, 1.0f)) {

    this->resize(scene.getWindowWidth(), scene.getWindowHeight());

    // Only do this once, as we have a single shader program
    this->pipeline.use();

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    this->UI.setup(this);
    this->UI.setCamera(&this->scene.getCamera());

    this->UI.setCameraUpdateCallbacks(
        [this](const glm::vec3 &newPos) { this->scene.setCameraPosition(newPos); },
        [this](const std::string &newType) {

        });
}

void SceneWindow::onUpdate(float time, float timeElapsed) {
    static_cast<void>(time);

    GLFWwindow *windowHandle = this->getHandle();
    camera::Camera &baseCamera = scene.getCamera();

    std::map<int, camera::MovementDirection> keyMapping;

    camera::OrbitalCamera *orbitalCamera = dynamic_cast<camera::OrbitalCamera *>(&baseCamera);
    if (orbitalCamera) {
        keyMapping = {
            { GLFW_KEY_W, camera::MovementDirection::Up       },
            { GLFW_KEY_S, camera::MovementDirection::Down     },
            { GLFW_KEY_A, camera::MovementDirection::Left     },
            { GLFW_KEY_D, camera::MovementDirection::Right    },
            { GLFW_KEY_F, camera::MovementDirection::Forward  },
            { GLFW_KEY_B, camera::MovementDirection::Backward }
        };
    }

    camera::FreeCamera *freeCamera = dynamic_cast<camera::FreeCamera *>(&baseCamera);
    if (freeCamera) {
        keyMapping = {
            { GLFW_KEY_W,          camera::MovementDirection::Forward   },
            { GLFW_KEY_S,          camera::MovementDirection::Backward  },
            { GLFW_KEY_A,          camera::MovementDirection::Left      },
            { GLFW_KEY_D,          camera::MovementDirection::Right     },
            { GLFW_KEY_SPACE,      camera::MovementDirection::Up        },
            { GLFW_KEY_LEFT_SHIFT, camera::MovementDirection::Down      },
            { GLFW_KEY_LEFT,       camera::MovementDirection::LookLeft  },
            { GLFW_KEY_RIGHT,      camera::MovementDirection::LookRight },
            { GLFW_KEY_UP,         camera::MovementDirection::LookUp    },
            { GLFW_KEY_DOWN,       camera::MovementDirection::LookDown  }
        };
    }

    for (const auto &[key, direction] : keyMapping) {
        if (glfwGetKey(windowHandle, key) == GLFW_PRESS) {
            baseCamera.move(direction, timeElapsed);
        }
    }
}

void SceneWindow::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    this->UI.render();
    this->scene.draw(this->pipeline);

    if (this->UI.isShowAxesEnabled()) {
        this->xAxis.draw(this->pipeline);
        this->yAxis.draw(this->pipeline);
        this->zAxis.draw(this->pipeline);
    }
}

void SceneWindow::onResize(int _width, int _height) {
    glViewport(0, 0, _width, _height);
    scene.setWindowSize(_width, _height);
}

}
