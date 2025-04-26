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

#include <filesystem>
#include <numeric>
#include <tinyxml2.h>
#include <unordered_map>

#include "engine/scene/camera/CameraFactory.hpp"
#include "engine/scene/Scene.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::scene {

Scene::Scene(const std::string &file) :
    xAxis(glm::vec3(1.0f, 0.0f, 0.0f)),
    yAxis(glm::vec3(0.0f, 1.0f, 0.0f)),
    zAxis(glm::vec3(0.0f, 0.0f, 1.0f)) {

    const std::filesystem::path sceneDirectory = std::filesystem::path(file).parent_path();
    std::unordered_map<std::string, std::shared_ptr<render::Model>> loadedModels;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(file.c_str()) != tinyxml2::XML_SUCCESS) {
        throw std::runtime_error("Failed to open / parse scene XML file");
    }

    const tinyxml2::XMLElement *worldElement = utils::XMLUtils::getSingleChild(&doc, "world");

    // Get window properties
    const tinyxml2::XMLElement *windowElement =
        utils::XMLUtils::getSingleChild(worldElement, "window");

    this->windowWidth = windowElement->IntAttribute("width", -1);
    this->windowHeight = windowElement->IntAttribute("height", -1);
    if (this->windowWidth < 0 || this->windowHeight < 0) {
        throw std::runtime_error("Invalid / unknown window width / height in scene XML file");
    }

    // Get camera properties
    this->camera = camera::CameraFactory::createFromXML(
        utils::XMLUtils::getSingleChild(worldElement, "camera"),
        sceneDirectory,
        loadedModels);

    // Get rendering groups
    const tinyxml2::XMLElement *groupElement = worldElement->FirstChildElement("group");
    while (groupElement) {
        this->groups.push_back(std::make_unique<Group>(groupElement, sceneDirectory, loadedModels));
        groupElement = groupElement->NextSiblingElement("group");
    }
}

int Scene::getWindowWidth() const {
    return this->windowWidth;
}

int Scene::getWindowHeight() const {
    return this->windowHeight;
}

int Scene::getEntityCount() const {
    return std::transform_reduce(
               this->groups.cbegin(),
               this->groups.cend(),
               0,
               std::plus<>(),
               [](const std::unique_ptr<Group> &group) { return group->getEntityCount(); }) +
        this->camera->getEntityCount();
}

camera::Camera &Scene::getCamera() {
    return *camera;
}

void Scene::setWindowSize(int width, int height) {
    this->windowWidth = width;
    this->windowHeight = height;
    this->camera->setWindowSize(width, height);
}

void Scene::update(float time) {
    for (const std::unique_ptr<Group> &group : this->groups) {
        group->update(time);
    }
}

int Scene::draw(render::RenderPipelineManager &pipelineManager,
                bool fillPolygons,
                bool backFaceCulling,
                bool showAxes,
                bool showBoundingSpheres,
                bool showAnimationLines) const {

    if (backFaceCulling) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }

    // Draw scene contents
    int entityCount = 0;
    entityCount += this->camera->draw(pipelineManager, fillPolygons, showBoundingSpheres);

    const glm::mat4 &cameraMatrix = this->camera->getCameraMatrix();
    for (const std::unique_ptr<Group> &group : this->groups) {
        group->updateBoundingSphere(glm::mat4(1.0f));
        entityCount += group->draw(pipelineManager,
                                   *this->camera,
                                   cameraMatrix,
                                   fillPolygons,
                                   showBoundingSpheres,
                                   showAnimationLines);
    }

    // Draw axes
    if (showAxes) {
        this->xAxis.draw(pipelineManager, cameraMatrix);
        this->yAxis.draw(pipelineManager, cameraMatrix);
        this->zAxis.draw(pipelineManager, cameraMatrix);
    }

    return entityCount;
}

}
