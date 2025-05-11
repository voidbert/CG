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

#include "engine/render/PickingShaderProgram.hpp"
#include "engine/render/ShadedShaderProgram.hpp"
#include "engine/render/ShaderProgram.hpp"
#include "engine/render/SolidColorShaderProgram.hpp"

namespace engine::render {

class RenderPipelineManager {
private:
    ShadedShaderProgram shadedShaderProgram;
    SolidColorShaderProgram solidColorShaderProgram;
    PickingShaderProgram pickingShaderProgram;
    ShaderProgram *currentProgram;
    bool currentfillPolygons;

public:
    RenderPipelineManager(int pointLights, int directionalLights, int spotlights);
    RenderPipelineManager(const RenderPipelineManager &model) = delete;
    RenderPipelineManager(RenderPipelineManager &&) = delete;

    void setFillPolygons(bool fillPolygons);

    const SolidColorShaderProgram &getSolidColorShaderProgram();
    const ShadedShaderProgram &getShadedShaderProgram();

    PickingShaderProgram &getPickingShader();

private:
    void useProgram(ShaderProgram *program);
};

}
