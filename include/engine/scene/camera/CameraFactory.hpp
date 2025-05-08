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

#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <tinyxml2.h>
#include <unordered_map>

#include "engine/render/Model.hpp"
#include "engine/render/Texture.hpp"
#include "engine/scene/camera/Camera.hpp"

namespace engine::scene::camera {

class CameraFactory {
public:
    static std::unique_ptr<Camera> createFromXML(
        const tinyxml2::XMLElement *cameraElement,
        const std::filesystem::path &sceneDirectory,
        std::unordered_map<std::string, std::shared_ptr<render::Model>> &loadedModels,
        std::unordered_map<std::string, std::shared_ptr<render::Texture>> &loadedTextures);
};

}
