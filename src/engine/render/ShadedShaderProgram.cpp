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
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <regex>
#include <sstream>
#include <stdexcept>

#include "engine/render/ShadedShaderProgram.hpp"
#include "engine/scene/light/DirectionalLight.hpp"
#include "engine/scene/light/PointLight.hpp"
#include "engine/scene/light/Spotlight.hpp"

namespace engine::render {

ShadedShaderProgram::ShadedShaderProgram(int _pointLights,
                                         int _directionalLights,
                                         int _spotlights) :
    ShaderProgram(ShadedShaderProgram::vertexShaderSource,
                  ShadedShaderProgram::initializeFragmentShader(_pointLights,
                                                                _directionalLights,
                                                                _spotlights)),
    pointLights(_pointLights),
    directionalLights(_directionalLights),
    spotlights(_spotlights),
    fullMatrixUniformLocation(this->getUniformLocation("uniFullMatrix")),
    worldMatrixUniformLocation(this->getUniformLocation("uniWorldMatrix")),
    normalMatrixUniformLocation(this->getUniformLocation("uniNormalMatrix")),
    cameraPositionUniformLocation(this->getUniformLocation("uniCameraPosition")),
    texturedUniformLocation(this->getUniformLocation("uniTextured")),
    diffuseUniformLocation(this->getUniformLocation("uniDiffuse")),
    ambientUniformLocation(this->getUniformLocation("uniAmbient")),
    specularUniformLocation(this->getUniformLocation("uniSpecular")),
    emissiveUniformLocation(this->getUniformLocation("uniEmissive")),
    shininessUniformLocation(this->getUniformLocation("uniShininess")),
    pointPositionsUniformLocation(this->getUniformLocation("uniPointPositions")),
    directionalDirectionsUniformLocation(this->getUniformLocation("uniDirectionalDirections")),
    spotPositionsUniformLocation(this->getUniformLocation("uniSpotPositions")),
    spotDirectionsUniformLocation(this->getUniformLocation("uniSpotDirections")),
    spotCutoffsUniformLocation(this->getUniformLocation("uniSpotCutoffs")) {}

void ShadedShaderProgram::setFullMatrix(const glm::mat4 &fullMatrix) const {
    glUniformMatrix4fv(this->fullMatrixUniformLocation, 1, false, glm::value_ptr(fullMatrix));
}

void ShadedShaderProgram::setWorldMatrix(const glm::mat4 &worldMatrix) const {
    glUniformMatrix4fv(this->worldMatrixUniformLocation, 1, false, glm::value_ptr(worldMatrix));
}

void ShadedShaderProgram::setNormalMatrix(const glm::mat4 &normalMatrix) const {
    glUniformMatrix4fv(this->normalMatrixUniformLocation, 1, false, glm::value_ptr(normalMatrix));
}

void ShadedShaderProgram::setCameraPosition(const glm::vec3 &position) const {
    glUniform3f(this->cameraPositionUniformLocation, position.x, position.y, position.z);
}

void ShadedShaderProgram::setTexture(const Texture &texture,
                                     const scene::Material &material) const {
    this->setMaterial(material);
    glUniform1i(this->texturedUniformLocation, true);
    texture.use();
}

void ShadedShaderProgram::setMaterial(const scene::Material &material) const {
    glUniform1i(this->texturedUniformLocation, false);

    const glm::vec3 &diffuse = material.getDiffuse();
    glUniform3f(this->diffuseUniformLocation, diffuse.r, diffuse.g, diffuse.b);

    const glm::vec3 &ambient = material.getAmbient();
    glUniform3f(this->ambientUniformLocation, ambient.r, ambient.g, ambient.b);

    const glm::vec3 &specular = material.getSpecular();
    glUniform3f(this->specularUniformLocation, specular.r, specular.g, specular.b);

    const glm::vec3 &emissive = material.getEmissive();
    glUniform3f(this->emissiveUniformLocation, emissive.r, emissive.g, emissive.b);

    glUniform1f(this->shininessUniformLocation, material.getShininess());
}

void ShadedShaderProgram::setLights(
    const std::vector<std::unique_ptr<scene::light::Light>> &lights) const {
    int pointLightCount = 0, directionalLightCount = 0, spotlightCount = 0;

    for (const std::unique_ptr<scene::light::Light> &light : lights) {
        if (dynamic_cast<scene::light::PointLight *>(light.get())) {
            scene::light::PointLight pointLight = dynamic_cast<scene::light::PointLight &>(*light);

            const glm::vec3 &position = pointLight.getPosition();
            glUniform3f(this->pointPositionsUniformLocation + pointLightCount,
                        position.x,
                        position.y,
                        position.z);

            pointLightCount++;
        } else if (dynamic_cast<scene::light::DirectionalLight *>(light.get())) {
            scene::light::DirectionalLight directionalLight =
                dynamic_cast<scene::light::DirectionalLight &>(*light);

            const glm::vec3 &direction = directionalLight.getDirection();
            glUniform3f(this->directionalDirectionsUniformLocation + directionalLightCount,
                        direction.x,
                        direction.y,
                        direction.z);

            directionalLightCount++;
        } else if (dynamic_cast<scene::light::Spotlight *>(light.get())) {
            scene::light::Spotlight spotlight = dynamic_cast<scene::light::Spotlight &>(*light);

            const glm::vec3 &position = spotlight.getPosition();
            glUniform3f(this->spotPositionsUniformLocation + spotlightCount,
                        position.x,
                        position.y,
                        position.z);

            const glm::vec3 &direction = spotlight.getDirection();
            glUniform3f(this->spotDirectionsUniformLocation + spotlightCount,
                        direction.x,
                        direction.y,
                        direction.z);

            const float cutoffAngle = spotlight.getCutoff();
            glUniform1f(this->spotCutoffsUniformLocation + spotlightCount, cosf(cutoffAngle));

            spotlightCount++;
        }
    }

    if (pointLightCount != this->pointLights || directionalLightCount != this->directionalLights ||
        spotlightCount != this->spotlights) {

        throw std::runtime_error("ShadedShaderProgram got wrong number of lights");
    }
}

std::string ShadedShaderProgram::initializeFragmentShader(int _pointLights,
                                                          int _directionalLights,
                                                          int _spotlights) {
    std::stringstream ss;
    ss << "#version 460 core" << std::endl;
    ss << "#define NUM_DIRECTIONAL_LIGHTS " << _directionalLights << std::endl;
    ss << "#define NUM_POINT_LIGHTS " << _pointLights << std::endl;
    ss << "#define NUM_SPOTLIGHTS " << _spotlights << std::endl;
    ss << ShadedShaderProgram::fragmentShaderSource;
    return ss.str();
}

const std::string ShadedShaderProgram::vertexShaderSource = R"(
#version 460 core

layout (location = 0) in vec4 inPosition;          // Local space
layout (location = 1) in vec2 inTextureCoordinate;
layout (location = 2) in vec3 inNormal;            // Local space

layout (location = 0) out vec2 outTextureCoordinate;
layout (location = 1) out vec3 outNormal;            // World space
layout (location = 2) out vec3 outFragmentPosition;  // World space

uniform mat4 uniFullMatrix;   // PVM
uniform mat4 uniWorldMatrix;  // M
uniform mat4 uniNormalMatrix; // (M^T)^(-1)

void main() {
    gl_Position = uniFullMatrix * inPosition;                // Clip space
    outTextureCoordinate = inTextureCoordinate;
    outNormal = normalize(mat3(uniNormalMatrix) * inNormal); // World space
    outFragmentPosition = vec3(uniWorldMatrix * inPosition); // World space
}
)";

const std::string ShadedShaderProgram::fragmentShaderSource = R"(
layout (location = 0) in vec2 inTextureCoordinate;
layout (location = 1) in vec3 inNormal;            // World space
layout (location = 2) in vec3 inFragmentPosition;  // World space

layout (location = 0) out vec4 outColor;

uniform vec3 uniCameraPosition; // World space
uniform bool uniTextured;
uniform vec3 uniDiffuse;
uniform vec3 uniAmbient;
uniform vec3 uniSpecular;
uniform vec3 uniEmissive;
uniform float uniShininess;

struct ColorPair {
    vec3 regularColor;
    vec3 specularColor;
};

ColorPair light(vec3 normal, vec3 lightDirection, vec3 cameraDirection) {
    ColorPair ret = ColorPair(vec3(0.0f), vec3(0.0f));

    float intensity = max(dot(normal, lightDirection), 0.0);
    ret.regularColor = intensity * uniDiffuse;

    if (intensity > 0.0f && uniShininess > 0.0f) {
        vec3 reflectDirection = reflect(-lightDirection, normal);
        float specularIntensity = max(dot(cameraDirection, reflectDirection), 0.0);
        ret.specularColor = uniSpecular * pow(specularIntensity, uniShininess);
    }

    return ret;
}

#if NUM_POINT_LIGHTS > 0
uniform vec3 uniPointPositions[NUM_POINT_LIGHTS]; // World space

ColorPair point(vec3 normal, vec3 cameraDirection) {
    ColorPair ret = ColorPair(vec3(0.0f), vec3(0.0f));
    for (uint i = 0; i < NUM_POINT_LIGHTS; ++i) {
        vec3 lightDirection = normalize(uniPointPositions[i] - inFragmentPosition);

        ColorPair lightColor = light(normal, lightDirection, cameraDirection);
        ret.regularColor += lightColor.regularColor;
        ret.specularColor += lightColor.specularColor;
    }
    return ret;
}
#else
ColorPair point(vec3 normal, vec3 cameraDirection) {
    return ColorPair(vec3(0.0f), vec3(0.0f));
}
#endif

#if NUM_DIRECTIONAL_LIGHTS > 0
uniform vec3 uniDirectionalDirections[NUM_DIRECTIONAL_LIGHTS]; // World space

ColorPair directional(vec3 normal, vec3 cameraDirection) {
    ColorPair ret = ColorPair(vec3(0.0f), vec3(0.0f));
    for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; ++i) {
        ColorPair lightColor = light(normal, uniDirectionalDirections[i], cameraDirection);
        ret.regularColor += lightColor.regularColor;
        ret.specularColor += lightColor.specularColor;
    }
    return ret;
}
#else
ColorPair directional(vec3 normal, vec3 cameraDirection) {
    return ColorPair(vec3(0.0f), vec3(0.0f));
}
#endif

#if NUM_SPOTLIGHTS > 0
uniform vec3 uniSpotPositions[NUM_SPOTLIGHTS]; // World space
uniform vec3 uniSpotDirections[NUM_SPOTLIGHTS]; // World space
uniform float uniSpotCutoffs[NUM_SPOTLIGHTS];

ColorPair spot(vec3 normal, vec3 cameraDirection) {
    ColorPair ret = ColorPair(vec3(0.0f), vec3(0.0f));
    for (uint i = 0; i < NUM_SPOTLIGHTS; ++i) {
        vec3 lightDirection = normalize(uniSpotPositions[i] - inFragmentPosition);
        float angle = max(dot(lightDirection, -uniSpotDirections[i]), 0.0f);

        if (angle > uniSpotCutoffs[i]) {
            ColorPair lightColor = light(normal, lightDirection, cameraDirection);
            ret.regularColor += lightColor.regularColor;
            ret.specularColor += lightColor.specularColor;
        }
    }
    return ret;
}
#else
ColorPair spot(vec3 normal, vec3 cameraDirection) {
    return ColorPair(vec3(0.0f), vec3(0.0f));
}
#endif

uniform sampler2D uniSampler;

void main() {
    vec3 normal = normalize(inNormal);
    vec3 cameraDirection = normalize(uniCameraPosition - inFragmentPosition);

    ColorPair pointColors = point(normal, cameraDirection);
    ColorPair directionalColors = directional(normal, cameraDirection);
    ColorPair spotColors = spot(normal, cameraDirection);

    vec3 regularColor = pointColors.regularColor +
                        directionalColors.regularColor +
                        spotColors.regularColor +
                        uniAmbient +
                        uniEmissive;

    vec3 specularColor =
        pointColors.specularColor + directionalColors.specularColor + spotColors.specularColor;

    if (uniTextured) {
        vec3 textureColor = vec3(texture(uniSampler, inTextureCoordinate));
        outColor = vec4(textureColor * regularColor + specularColor, 1.0f);
    } else {
        outColor = vec4(regularColor + specularColor, 1.0f);
    }
}
)";

}
