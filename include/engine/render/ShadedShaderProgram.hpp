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
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>

#include "engine/render/ShaderProgram.hpp"
#include "engine/render/Texture.hpp"
#include "engine/scene/light/Light.hpp"
#include "engine/scene/Material.hpp"

namespace engine::render {

class ShadedShaderProgram : public ShaderProgram {
private:
    static const std::string vertexShaderSource, fragmentShaderSource;
    int pointLights, directionalLights, spotlights;

    GLint fullMatrixUniformLocation, worldMatrixUniformLocation, normalMatrixUniformLocation,
        cameraPositionUniformLocation, texturedUniformLocation, diffuseUniformLocation,
        ambientUniformLocation, specularUniformLocation, emissiveUniformLocation,
        shininessUniformLocation, pointPositionsUniformLocation,
        directionalDirectionsUniformLocation, spotPositionsUniformLocation,
        spotDirectionsUniformLocation, spotCutoffsUniformLocation;

public:
    ShadedShaderProgram(int _pointLights, int _directionalLights, int _spotlights);
    ShadedShaderProgram(const ShadedShaderProgram &program) = delete;
    ShadedShaderProgram(ShadedShaderProgram &&program) = delete;

    void setFullMatrix(const glm::mat4 &fullMatrix) const;
    void setWorldMatrix(const glm::mat4 &worldMatrix) const;
    void setNormalMatrix(const glm::mat4 &normalMatrix) const;
    void setCameraPosition(const glm::vec3 &position) const;

    void setTexture(const Texture &texture, const scene::Material &material) const;
    void setMaterial(const scene::Material &material) const;
    void setLights(const std::vector<std::unique_ptr<scene::light::Light>> &lights) const;

private:
    static std::string
        initializeFragmentShader(int pointLights, int directionalLights, int spotlights);
};

}
