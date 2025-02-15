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

#include <memory>

#include "engine/Model.hpp"
#include "engine/RenderPipeline.hpp"
#include "engine/Window.hpp"

namespace engine {
class SceneWindow : public Window {
private:
    RenderPipeline pipeline;

    // TODO - remove, these are for testing purposes only
    std::unique_ptr<Model> model1, model2;
    glm::vec3 translate;

public:
    SceneWindow();

protected:
    void onUpdate(float time, float timeElapsed);
    void onRender();
    void onResize(int _width, int _height);
};
}
