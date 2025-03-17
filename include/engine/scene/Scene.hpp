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

#include "engine/camera/Camera.hpp"
#include "engine/render/RenderPipeline.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/scene/Group.hpp"

namespace engine::scene {

class Scene {
private:
    int windowWidth, windowHeight;
    std::unique_ptr<camera::Camera> camera;
    std::vector<std::unique_ptr<Group>> groups;

public:
    explicit Scene(const std::string &file);
    Scene(const Scene &scene) = delete;
    Scene(Scene &&scene) = delete;

    int getWindowWidth() const;
    int getWindowHeight() const;
    void setWindowSize(int width, int height);
    camera::Camera &getCamera();

    void draw(const render::RenderPipeline &pipeline) const;
};

}
