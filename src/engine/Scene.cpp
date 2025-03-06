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

#include <cmath>

#include "engine/camera/Camera.hpp"
#include "engine/camera/CameraFactory.hpp"
#include "engine/Entity.hpp"
#include "engine/RenderPipeline.hpp"
#include "engine/Scene.hpp"
#include "utils/XMLUtils.hpp"

namespace engine {

Scene::Scene(const std::string &file) {
    const std::filesystem::path sceneDirectory = std::filesystem::path(file).parent_path();
    std::unordered_map<std::string, std::shared_ptr<Model>> loadedModels;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(file.c_str()) != tinyxml2::XML_SUCCESS) {
        throw std::runtime_error("Failed to load scene XML file");
    }

    const tinyxml2::XMLElement *worldElement = utils::XMLUtils::getSingleChild(&doc, "world");
    this->getWindowFromXML(worldElement);
    this->camera = camera::CameraFactory::createFromXML(
        utils::XMLUtils::getSingleChild(worldElement, "camera"));
    this->getEntitiesFromWorldXML(sceneDirectory, loadedModels, worldElement);
}

void Scene::getWindowFromXML(const tinyxml2::XMLElement *worldElement) {
    const tinyxml2::XMLElement *windowElement =
        utils::XMLUtils::getSingleChild(worldElement, "window");
    this->windowWidth = windowElement->IntAttribute("width", -1);
    this->windowHeight = windowElement->IntAttribute("height", -1);

    if (this->windowWidth < 0 || this->windowHeight < 0) {
        throw std::runtime_error("Invalid / unknown window width / height in scene XML file");
    }
}

void Scene::getEntitiesFromWorldXML(
    const std::filesystem::path &sceneDirectory,
    std::unordered_map<std::string, std::shared_ptr<Model>> &loadedModels,
    const tinyxml2::XMLElement *worldElement) {

    const tinyxml2::XMLElement *groupElement = worldElement->FirstChildElement("group");
    while (groupElement) {
        this->getEntitiesFromGroupXML(sceneDirectory, loadedModels, groupElement);
        groupElement = worldElement->NextSiblingElement("group");
    }
}

void Scene::getEntitiesFromGroupXML(
    const std::filesystem::path &sceneDirectory,
    std::unordered_map<std::string, std::shared_ptr<Model>> &loadedModels,
    const tinyxml2::XMLElement *groupElement) {

    const tinyxml2::XMLElement *innerGroupElement = groupElement->FirstChildElement("group");
    while (innerGroupElement) {
        this->getEntitiesFromGroupXML(sceneDirectory, loadedModels, innerGroupElement);
        innerGroupElement = innerGroupElement->NextSiblingElement("group");
    }

    const tinyxml2::XMLElement *modelsElement = groupElement->FirstChildElement("models");
    if (!modelsElement)
        return; // <models> is optional

    const tinyxml2::XMLElement *modelElement = modelsElement->FirstChildElement("model");
    while (modelElement) {
        const char *file = modelElement->Attribute("file");
        if (!file) {
            throw std::runtime_error("Invalid <model> in scene XML file");
        }

        std::string modelPath = std::filesystem::canonical(sceneDirectory / file);
        auto it = loadedModels.find(modelPath);
        std::shared_ptr<Model> model;
        if (it == loadedModels.end()) {
            utils::WavefrontOBJ object(modelPath);
            model = std::make_shared<Model>(object);
            loadedModels[modelPath] = model;
        } else {
            model = it->second;
        }

        // TODO - color
        std::unique_ptr<Entity> entity = std::make_unique<Entity>(model, glm::vec4(1.0f));
        entities.push_back(std::move(entity));

        modelElement = modelElement->NextSiblingElement("model");
    }
}

void Scene::draw(const RenderPipeline &pipeline) const {
    for (const std::unique_ptr<Entity> &entity : this->entities) {
        entity->draw(pipeline);
    }
}

int Scene::getWindowWidth() const {
    return this->windowWidth;
}

int Scene::getWindowHeight() const {
    return this->windowHeight;
}

camera::Camera &Scene::getCamera() {
    return *camera;
}

void Scene::setWindowSize(int width, int height) {
    this->windowWidth = width;
    this->windowHeight = height;
}

}
