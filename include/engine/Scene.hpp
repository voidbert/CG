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

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <string>
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
    std::vector<std::unique_ptr<engine::Entity>> entities;

public:
    Scene();
    Scene(const int &argWindowWidth,
          const int &argWindowHeight,
          const std::string &argWindowTitle,
          const Camera &argCamera,
          std::vector<std::unique_ptr<engine::Entity>> argEntities);
    void addEntity(std::unique_ptr<engine::Entity> entity);
    void draw(const RenderPipeline &pipeline);
    void updateCamera(float timeElapsed, const glm::vec3 &direction);
    glm::mat4 getCameraMatrix() const;
    void setWindowSize(int width, int height);

    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    Scene(Scene &&other) noexcept;
    Scene &operator=(Scene &&other) noexcept;
};
}
