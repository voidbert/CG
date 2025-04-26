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
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "engine/scene/camera/CameraFactory.hpp"
#include "engine/scene/camera/FreeCamera.hpp"
#include "engine/scene/camera/OrbitalCamera.hpp"
#include "engine/scene/camera/ThirdPersonCamera.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::scene::camera {

std::unique_ptr<Camera> CameraFactory::createFromXML(
    const tinyxml2::XMLElement *cameraElement,
    const std::filesystem::path &sceneDirectory,
    std::unordered_map<std::string, std::shared_ptr<render::Model>> &loadedModels) {

    // View matrix
    const glm::vec3 position =
        utils::XMLUtils::getXYZ(utils::XMLUtils::getSingleChild(cameraElement, "position"));
    const glm::vec3 lookAt =
        utils::XMLUtils::getXYZ(utils::XMLUtils::getSingleChild(cameraElement, "lookAt"));

    glm::vec3 up(0.0f, 1.0f, 0.0f);
    try {
        up = utils::XMLUtils::getXYZ(utils::XMLUtils::getSingleChild(cameraElement, "up"));
    } catch (std::runtime_error &) {}

    // Projection matrix
    float fov = glm::radians(60.0f), near = 1.0f, far = 1000.0f;

    const tinyxml2::XMLElement *projectionElement = cameraElement->FirstChildElement("projection");
    if (projectionElement) {
        fov = glm::radians(projectionElement->FloatAttribute("fov", fov));
        near = projectionElement->FloatAttribute("near", near);
        far = projectionElement->FloatAttribute("far", far);
    }

    // Type of camera
    std::string cameraType = "orbital";
    const char *cameraTypePtr = cameraElement->Attribute("type");
    if (cameraTypePtr) {
        cameraType = cameraTypePtr;
    }

    if (cameraType == "orbital") {
        return std::make_unique<camera::OrbitalCamera>(position, lookAt, up, fov, near, far);
    } else if (cameraType == "free") {
        return std::make_unique<camera::FreeCamera>(position, lookAt, up, fov, near, far);
    } else if (cameraType == "thirdperson") {
        const tinyxml2::XMLElement *group = utils::XMLUtils::getSingleChild(cameraElement, "group");
        std::unique_ptr<scene::Group> player =
            std::make_unique<scene::Group>(group, sceneDirectory, loadedModels);

        return std::make_unique<ThirdPersonCamera>(position,
                                                   lookAt,
                                                   up,
                                                   fov,
                                                   near,
                                                   far,
                                                   std::move(player));
    } else {
        throw std::runtime_error("Invalid camera type in scene XML file");
    }
}

}
