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
#include <glm/gtc/type_ptr.hpp>
#include <iostream> // TODO - remove
#include <regex>

#include "engine/render/ShadedShaderProgram.hpp"
#include "engine/scene/light/DirectionalLight.hpp"
#include "engine/scene/light/PointLight.hpp"
#include "engine/scene/light/Spotlight.hpp"

namespace engine::render {

ShadedShaderProgram::ShadedShaderProgram(int pointLights, int directionalLights, int spotlights) :
    ShaderProgram(
        ShadedShaderProgram::vertexShaderSource,
        ShadedShaderProgram::getFragmentShaderSource(pointLights, directionalLights, spotlights)) {}

void ShadedShaderProgram::setFullMatrix(const glm::mat4 &fullMatrix) const {
    glUniformMatrix4fv(0, 1, false, glm::value_ptr(fullMatrix));
}

void ShadedShaderProgram::setWorldMatrix(const glm::mat4 &worldMatrix) const {
    glUniformMatrix4fv(1, 1, false, glm::value_ptr(worldMatrix));
}

void ShadedShaderProgram::setNormalMatrix(const glm::mat4 &normalMatrix) const {
    glUniformMatrix4fv(2, 1, false, glm::value_ptr(normalMatrix));
}

void ShadedShaderProgram::setCameraPosition(const glm::vec3 &position) const {
    glUniform3f(3, position.x, position.y, position.z);
}

void ShadedShaderProgram::setTexture(const Texture &texture) const {
    this->setMaterial(scene::Material());
    glUniform1i(4, true);
    texture.use();
}

void ShadedShaderProgram::setMaterial(const scene::Material &material) const {
    glUniform1i(4, false);

    const glm::vec3 &diffuse = material.getDiffuse();
    glUniform3f(5, diffuse.r, diffuse.g, diffuse.b);

    const glm::vec3 &ambient = material.getAmbient();
    glUniform3f(6, ambient.r, ambient.g, ambient.b);

    const glm::vec3 &specular = material.getSpecular();
    glUniform3f(7, specular.r, specular.g, specular.b);

    const glm::vec3 &emissive = material.getEmissive();
    glUniform3f(8, emissive.r, emissive.g, emissive.b);

    glUniform1f(9, material.getShininess());
}

void ShadedShaderProgram::setLights(
    const std::vector<std::unique_ptr<scene::light::Light>> &lights) const {
    int pointLights = 0, directionalLights = 0, spotlights = 0;

    for (const std::unique_ptr<scene::light::Light> &light : lights) {
        if (dynamic_cast<scene::light::DirectionalLight *>(light.get())) {
            scene::light::DirectionalLight directionalLight =
                dynamic_cast<scene::light::DirectionalLight &>(*light);

            const glm::vec3 &direction = directionalLight.getDirection();
            glUniform3f(10 + directionalLights, direction.x, direction.y, direction.z);
            directionalLights++;
        }
    }
}

std::string ShadedShaderProgram::getFragmentShaderSource(int pointLights,
                                                         int directionalLights,
                                                         int spotlights) {
    std::string r = std::string("#version 460 core\n") + "\n#define NUM_POINT_LIGHTS " +
        std::to_string(pointLights) + "\n#define NUM_DIRECTIONAL_LIGHTS " +
        std::to_string(directionalLights) + "\n#define NUM_SPOTLIGHTS " +
        std::to_string(spotlights) + "\n" + ShadedShaderProgram::fragmentShaderSource;

    std::cout << r << std::endl;
    return r;
}

const std::string ShadedShaderProgram::vertexShaderSource = R"(
#version 460 core

layout (location = 0) in vec4 inPosition;          // Local space
layout (location = 1) in vec2 inTextureCoordinate;
layout (location = 2) in vec3 inNormal;            // Local space

layout (location = 0) out vec2 outTextureCoordinate;
layout (location = 1) out vec3 outNormal;            // World space
layout (location = 2) out vec3 outFragmentPosition;  // World space

layout (location = 0) uniform mat4 uniFullMatrix;   // PVM
layout (location = 1) uniform mat4 uniWorldMatrix;  // M
layout (location = 2) uniform mat4 uniNormalMatrix; // (M^T)^(-1)

void main() {
    gl_Position = uniFullMatrix * inPosition;
    outTextureCoordinate = inTextureCoordinate;
    outNormal = normalize(mat3(uniNormalMatrix) * inNormal);
    outFragmentPosition = vec3(uniWorldMatrix * inPosition);
}
)";

const std::string ShadedShaderProgram::fragmentShaderSource = R"(
layout (location = 0) in vec2 inTextureCoordinate;
layout (location = 1) in vec3 inNormal;            // World space
layout (location = 2) in vec3 inFragmentPosition;  // World space

layout (location = 0) out vec4 outColor;

layout (location = 3) uniform vec3 uniCameraPosition; // World space
layout (location = 4) uniform bool uniTextured;
layout (location = 5) uniform vec3 uniDiffuse;
layout (location = 6) uniform vec3 uniAmbient;
layout (location = 7) uniform vec3 uniSpecular;
layout (location = 8) uniform vec3 uniEmissive;
layout (location = 9) uniform float uniShininess;

struct ColorPair {
    vec3 regularColor;
    vec3 specularColor;
};

#if NUM_DIRECTIONAL_LIGHTS > 0

layout (location = 10) uniform vec3 uniDirectionalDirections[NUM_DIRECTIONAL_LIGHTS]; // World space

ColorPair directional(vec3 normal, vec3 cameraDirection) {
    ColorPair ret;
    ret.regularColor = vec3(0.0f, 0.0f, 0.0f);
    ret.specularColor = vec3(0.0f, 0.0f, 0.0f);

    for (uint i = 0; i < NUM_DIRECTIONAL_LIGHTS; ++i) {
        vec3 lightDirection = uniDirectionalDirections[i];
        float intensity = max(dot(normal, lightDirection), 0.0);
        ret.regularColor += intensity * uniDiffuse;

        if (intensity > 0.0f) {
            vec3 reflectDirection = reflect(-lightDirection, normal);
            float specularIntensity = max(dot(cameraDirection, reflectDirection), 0.0);
            ret.specularColor += uniSpecular * pow(specularIntensity, uniShininess);
        }
    }

    return ret;
}

#else

ColorPair directional(vec3 normal, vec3 cameraDirection) {
    ColorPair ret;
    ret.regularColor = vec3(0.0f, 0.0f, 0.0f);
    ret.specularColor = vec3(0.0f, 0.0f, 0.0f);
    return ret;
}
#endif

uniform sampler2D uniSampler;

void main() {
    vec3 normal = normalize(inNormal);
    vec3 cameraDirection = normalize(uniCameraPosition - inFragmentPosition);

    ColorPair directionalColors = directional(normal, cameraDirection);

    vec3 regularColor = directionalColors.regularColor + uniAmbient + uniEmissive;
    vec3 specularColor = directionalColors.specularColor;

    if (uniTextured) {
        vec3 textureColor = vec3(texture(uniSampler, inTextureCoordinate));
        outColor = vec4(textureColor * regularColor + specularColor, 1.0f);
    } else {
        // NOTE: To get output equal to the provided examples, add to ambient color (instead of
        // using max())
        outColor = vec4(regularColor + specularColor, 1.0f);
    }
}
)";

}
