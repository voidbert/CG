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

#include <glm/vec3.hpp>

#include "engine/camera/CameraFactory.hpp"
#include "engine/camera/OrbitalCamera.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::camera {

std::unique_ptr<Camera> CameraFactory::createFromXML(const tinyxml2::XMLElement *cameraElement) {
    // View matrix
    const glm::vec3 position =
        utils::XMLUtils::getXYZ(utils::XMLUtils::getSingleChild(cameraElement, "position"));
    const glm::vec3 lookAt =
        utils::XMLUtils::getXYZ(utils::XMLUtils::getSingleChild(cameraElement, "lookAt"));
    const glm::vec3 up =
        utils::XMLUtils::getXYZ(utils::XMLUtils::getSingleChild(cameraElement, "up"));

    // Projection matrix
    const tinyxml2::XMLElement *projectionElement =
        utils::XMLUtils::getSingleChild(cameraElement, "projection");

    const float fov = projectionElement->FloatAttribute("fov", NAN);
    const float near = projectionElement->FloatAttribute("near", NAN);
    const float far = projectionElement->FloatAttribute("far", NAN);

    if (std::isnan(fov) || std::isnan(near) || std::isnan(far)) {
        throw std::runtime_error("Invalid <projection> in scene XML file");
    }

    // Type of camera
    std::string cameraType = cameraElement->Attribute("type");

    if (cameraType == "orbital") {
        return std::make_unique<camera::OrbitalCamera>(position, lookAt, up, fov, near, far);
    } else if (cameraType == "free") {
        // TODO - Sara - FreeCamera
        return std::unique_ptr<camera::Camera>(nullptr);
    } else if (cameraType == "thirdperson") {
        // TODO - Ana - ThirdPersonCamera
        return std::unique_ptr<camera::Camera>(nullptr);
    } else {
        throw std::runtime_error("Invalid camera type in scene XML file");
    }
}

}
