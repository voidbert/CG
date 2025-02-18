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

#include <iostream>
#include <tinyxml2.h>

#include "engine/Scene.hpp"
#include "utils/Parse.hpp"

namespace utils {

engine::Scene configScene(const std::string &filename) {
    if (filename.size() > 4 && filename.compare(filename.size() - 4, 4, ".xml") == 0) {
        return parseSceneXML(filename);
    } else if ((filename.size() > 3 && filename.compare(filename.size() - 3, 3, ".3d") == 0) ||
               (filename.size() > 4 && filename.compare(filename.size() - 4, 4, ".obj") == 0)) {
        return parseSceneOBJ(filename);
    } else {
        throw std::runtime_error("Invalid file format");
    }
}

engine::Scene parseSceneXML(const std::string &filename) {
    engine::Scene scene;
    std::vector<std::unique_ptr<engine::Entity>> entities;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        throw std::runtime_error("Failed to load XML file");
    }

    tinyxml2::XMLElement *worldElement = doc.FirstChildElement("world");
    if (!worldElement) {
        throw std::runtime_error("<world> element not found");
    }

    int width;
    int height;
    tinyxml2::XMLElement *windowElement = worldElement->FirstChildElement("window");
    if (windowElement) {
        width = windowElement->IntAttribute("width", 800);
        height = windowElement->IntAttribute("height", 600);
    }

    glm::vec3 position(0.0f, 0.0f, 3.0f);
    glm::vec3 lookAt(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    float fov = 60.0;
    float near = 1.0;
    float far = 1000.0;
    tinyxml2::XMLElement *cameraElement = worldElement->FirstChildElement("camera");
    if (cameraElement) {
        tinyxml2::XMLElement *positionElement = cameraElement->FirstChildElement("position");
        if (positionElement) {
            position = glm::vec3(positionElement->FloatAttribute("x", 0.0f),
                                 positionElement->FloatAttribute("y", 0.0f),
                                 positionElement->FloatAttribute("z", 0.0f));
        }

        tinyxml2::XMLElement *lookAtElement = cameraElement->FirstChildElement("lookAt");
        if (lookAtElement) {
            lookAt = glm::vec3(lookAtElement->FloatAttribute("x", 0.0f),
                               lookAtElement->FloatAttribute("y", 0.0f),
                               lookAtElement->FloatAttribute("z", 0.0f));
        }

        tinyxml2::XMLElement *upElement = cameraElement->FirstChildElement("up");
        if (upElement) {
            up = glm::vec3(upElement->FloatAttribute("x", 0.0f),
                           upElement->FloatAttribute("y", 1.0f),
                           upElement->FloatAttribute("z", 0.0f));
        }
        tinyxml2::XMLElement *projectionElement = cameraElement->FirstChildElement("projection");
        if (projectionElement) {
            fov = projectionElement->FloatAttribute("fov", 60.0f);
            near = projectionElement->FloatAttribute("near", 1.0f);
            far = projectionElement->FloatAttribute("far", 1000.0f);
        }
    }

    engine::Camera camera(position, lookAt, up, fov, near, far);

    tinyxml2::XMLElement *groupElement = worldElement->FirstChildElement("group");
    if (groupElement) {
        processGroup(groupElement, glm::mat4(1.0f), entities);
    }

    return engine::Scene(width, height, "XML Scene", camera, std::move(entities));
}

void processGroup(tinyxml2::XMLElement *groupElement,
                  glm::mat4 parentTransform,
                  std::vector<std::unique_ptr<engine::Entity>> &entities) {
    // Remove comments in phase 2 and do a few changes, order is include
    glm::mat4 transform = parentTransform;

    /*tinyxml2::XMLElement* transformElement = groupElement->FirstChildElement("transform");
    if (transformElement) {
        std::vector<tinyxml2::XMLElement*> transforms;
        for (tinyxml2::XMLElement* elem = transformElement->FirstChildElement(); elem; elem =
    elem->NextSiblingElement()) { transforms.push_back(elem);
        }

        // Process transformations in reverse order
        std::reverse(transforms.begin(), transforms.end());

        for (auto& elem : transforms) {
            const char* name = elem->Name();
            if (strcmp(name, "translate") == 0) {
                glm::vec3 translation(
                    elem->FloatAttribute("x", 0.0f),
                    elem->FloatAttribute("y", 0.0f),
                    elem->FloatAttribute("z", 0.0f)
                );
                transform = glm::translate(transform, translation);
            } else if (strcmp(name, "rotate") == 0) {
                float angle = elem->FloatAttribute("angle", 0.0f);
                glm::vec3 axis(
                    elem->FloatAttribute("x", 0.0f),
                    elem->FloatAttribute("y", 0.0f),
                    elem->FloatAttribute("z", 0.0f)
                );
                transform = glm::rotate(transform, glm::radians(angle), axis);
            } else if (strcmp(name, "scale") == 0) {
                glm::vec3 scale(
                    elem->FloatAttribute("x", 1.0f),
                    elem->FloatAttribute("y", 1.0f),
                    elem->FloatAttribute("z", 1.0f)
                );
                transform = glm::scale(transform, scale);
            }
        }
    }*/

    tinyxml2::XMLElement *modelsElement = groupElement->FirstChildElement("models");
    if (modelsElement) {
        for (tinyxml2::XMLElement *modelElement = modelsElement->FirstChildElement("model");
             modelElement;
             modelElement = modelElement->NextSiblingElement("model")) {
            std::string modelFile = modelElement->Attribute("file");

            utils::WavefrontOBJ object(modelFile);
            auto model = std::make_unique<engine::Model>(object);

            auto entity = std::make_unique<engine::Entity>(std::move(model), glm::vec4(1.0f));
            entities.push_back(std::move(entity));
        }
    }

    for (tinyxml2::XMLElement *childGroupElement = groupElement->FirstChildElement("group");
         childGroupElement;
         childGroupElement = childGroupElement->NextSiblingElement("group")) {
        processGroup(childGroupElement, transform, entities);
    }
}

engine::Scene parseSceneOBJ(const std::string &filename) {
    const int &defaultWidth = 800;
    const int &defaultHeight = 600;
    std::string defaultTitle = "Scene";

    utils::WavefrontOBJ object(filename);
    auto model = std::make_unique<engine::Model>(object);

    auto entity =
        std::make_unique<engine::Entity>(std::move(model), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    std::vector<std::unique_ptr<engine::Entity>> entities;
    entities.push_back(std::move(entity));
    engine::Camera defaultCamera;
    engine::Scene scene(defaultWidth,
                        defaultHeight,
                        defaultTitle,
                        defaultCamera,
                        std::move(entities));
    return scene;
}

}
