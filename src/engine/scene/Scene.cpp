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

#include "engine/scene/Scene.hpp"

#include "engine/camera/CameraFactory.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::scene {

Scene::Scene(const std::string &file) {
    const std::filesystem::path sceneDirectory = std::filesystem::path(file).parent_path();
    std::unordered_map<std::string, std::shared_ptr<render::Model>> loadedModels;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(file.c_str()) != tinyxml2::XML_SUCCESS) {
        throw std::runtime_error("Failed to load scene XML file");
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
        utils::XMLUtils::getSingleChild(worldElement, "camera"));

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

void Scene::setWindowSize(int width, int height) {
    this->windowWidth = width;
    this->windowHeight = height;
}

camera::Camera &Scene::getCamera() {
    return *camera;
}

void Scene::draw(const render::RenderPipeline &pipeline) const {
    const float aspectRatio = static_cast<float>(this->windowWidth) / this->windowHeight;
    const glm::mat4 cameraMatrix = this->camera->getCameraMatrix(aspectRatio);

    for (const std::unique_ptr<Group> &group : this->groups) {
        group->draw(pipeline, cameraMatrix);
    }

    // Reset camera after transforms
    pipeline.setMatrix(cameraMatrix);
}

}
