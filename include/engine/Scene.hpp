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
#include <vector>

#include "engine/Camera.hpp"
#include "engine/Entity.hpp"
#include "engine/RenderPipeline.hpp"

namespace engine {

class Scene {
private:
    int windowWidth, windowHeight;
    std::string windowTitle;
    Camera camera;

    // TODO - Phase 2 - add support for groups (linear scene is going to make it harder for phase 3)
    std::vector<std::unique_ptr<Entity>> entities;

public:
    explicit Scene(const std::string &file);
    Scene(const Scene &scene) = delete;
    Scene(Scene &&scene) = delete;

    int getWindowWidth() const;
    int getWindowHeight() const;
    Camera &getCamera();

    void draw(const RenderPipeline &pipeline) const;
    void setWindowSize(int width, int height);

private:
    const tinyxml2::XMLElement *getOnlyOneNodeFromXML(const tinyxml2::XMLNode *parent,
                                                      const std::string &name);
    glm::vec3 getVectorFromXML(const tinyxml2::XMLElement *element);

    void getWindowFromXML(const tinyxml2::XMLElement *worldElement);
    void getCameraFromXML(const tinyxml2::XMLElement *worldElement);

    void getEntitiesFromWorldXML(
        const std::filesystem::path &sceneDirectory,
        std::unordered_map<std::string, std::shared_ptr<Model>> &loadedModels,
        const tinyxml2::XMLElement *worldElement);
    void getEntitiesFromGroupXML(
        const std::filesystem::path &sceneDirectory,
        std::unordered_map<std::string, std::shared_ptr<Model>> &loadedModels,
        const tinyxml2::XMLElement *groupdElement);
};

}
