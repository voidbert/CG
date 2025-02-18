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

#include "engine/Scene.hpp"

#include "engine/Camera.hpp"
#include "engine/Entity.hpp"
#include "engine/RenderPipeline.hpp"

namespace engine {

Scene::Scene() : windowWidth(800), windowHeight(600), windowTitle("Scene"), camera(), entities() {}

Scene::Scene(const int &argWindowWidth,
             const int &argWindowHeight,
             const std::string &argWindowTitle,
             const Camera &argCamera,
             std::vector<std::unique_ptr<Entity>> argEntities) :
    windowWidth(argWindowWidth),
    windowHeight(argWindowHeight),
    windowTitle(argWindowTitle),
    camera(argCamera),
    entities(std::move(argEntities)) {}

void Scene::addEntity(std::unique_ptr<Entity> entity) {
    entities.push_back(std::move(entity));
}

void Scene::draw(const RenderPipeline &pipeline) {
    for (auto &entity : entities) {
        entity->draw(pipeline);
    }
}

void Scene::updateCamera(float timeElapsed, const glm::vec3 &direction) {
    const float cameraSpeed = 2.5f;
    this->camera.move(direction * cameraSpeed, timeElapsed);
}

glm::mat4 Scene::getCameraMatrix() const {
    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    return camera.getCameraMatrix(aspectRatio);
}

Scene::Scene(Scene &&other) noexcept :
    windowWidth(other.windowWidth),
    windowHeight(other.windowHeight),
    windowTitle(std::move(other.windowTitle)),
    camera(std::move(other.camera)),
    entities(std::move(other.entities)) {}

Scene &Scene::operator=(Scene &&other) noexcept {
    if (this != &other) {
        windowWidth = other.windowWidth;
        windowHeight = other.windowHeight;
        windowTitle = std::move(other.windowTitle);
        camera = std::move(other.camera);
        entities = std::move(other.entities);
    }
    return *this;
}
void Scene::setWindowSize(int width, int height) {
    this->windowWidth = width;
    this->windowHeight = height;
}

}
