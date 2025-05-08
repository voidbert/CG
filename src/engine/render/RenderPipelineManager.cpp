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

#include <glad/glad.h>

#include "engine/render/RenderPipelineManager.hpp"

namespace engine::render {

RenderPipelineManager::RenderPipelineManager() :
    shadedShaderProgram(),
    solidColorShaderProgram(),
    currentProgram(nullptr),
    currentfillPolygons(true) {}

void RenderPipelineManager::setFillPolygons(bool fillPolygons) {
    if (this->currentfillPolygons != fillPolygons) {
        glPolygonMode(GL_FRONT_AND_BACK, fillPolygons ? GL_FILL : GL_LINE);
        this->currentfillPolygons = fillPolygons;
    }
}

const SolidColorShaderProgram &RenderPipelineManager::getSolidColorShaderProgram() {
    this->useProgram(&this->solidColorShaderProgram);
    return this->solidColorShaderProgram;
}

const ShadedShaderProgram &RenderPipelineManager::getShadedShaderProgram() {
    this->useProgram(&this->shadedShaderProgram);
    return this->shadedShaderProgram;
}

void RenderPipelineManager::useProgram(ShaderProgram *program) {
    if (this->currentProgram != program) {
        program->use();
        this->currentProgram = program;
    }
}

}
