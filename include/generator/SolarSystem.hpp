/// Copyright 2025 Ana Oliveira, Humberto Gomes, Mariana Rocha, Sara Lopes
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at:
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.

#pragma once

#include <glm/vec3.hpp>
#include <random>
#include <string>
#include <tinyxml2.h>

#include "utils/WavefrontOBJ.hpp"

namespace generator {

class SolarSystem {
private:
    std::default_random_engine rng;
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement *world;
    float bodyScale, lastTranslationAngle;

public:
    explicit SolarSystem(float sunScale = 1.0f, float rockyScale = 10.0f, float gasScale = 10.0f);

    void writeToFile(const std::string &dirname);

private:
    tinyxml2::XMLElement *createVector(const std::string &name, const glm::vec3 &vec);
    tinyxml2::XMLElement *
        createBody(float radius, float distance, bool hasOrbiters = false, float y = 0.0f);
    tinyxml2::XMLElement *createRings(float radius);
    tinyxml2::XMLElement *
        createAsteroidBelt(float minDistance, float maxDistance, int numAsteroids);

    void createPreamble(float sunScale, float rockyScale, float gasScale);
    void createCamera();
    void createObjects(float sunScale, float rockyScale, float gasScale);
};

}
