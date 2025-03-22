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

#include <cmath>
#include <string>
#include <tinyxml2.h>

#include "utils/WavefrontOBJ.hpp"

namespace generator::figures {

class SolarSystem : public utils::WavefrontOBJ {
private:
    tinyxml2::XMLDocument doc;

    void createWorld();
    void configureCamera();
    void generateSolarSystem(float sceneScale,
                             float sunSizeFactor,
                             float planetSizeFactor,
                             float moonSizeFactor,
                             float distanceFactor,
                             float asteroidBeltDensity,
                             float ringSizeFactor);

    tinyxml2::XMLElement *addCelestialBody(tinyxml2::XMLElement *parent,
                                           float x,
                                           float y,
                                           float z,
                                           float size,
                                           const std::string &modelFile,
                                           float angle = 0.0f,
                                           float rotX = 0.0f,
                                           float rotY = 0.0f,
                                           float rotZ = 0.0f);

    void addSatellite(tinyxml2::XMLElement *parent, float x, float y, float z, float size);

    void addRing(tinyxml2::XMLElement *parent,
                 float scaleX,
                 float scaleY,
                 float scaleZ,
                 float angle,
                 float rotX,
                 float rotY,
                 float rotZ);

    void addAsteroidBelt(tinyxml2::XMLElement *parent,
                         float minDist,
                         float maxDist,
                         int numAsteroids,
                         float sceneScale);

public:
    SolarSystem(float sceneScale,
                float sunSizeFactor,
                float planetSizeFactor,
                float moonSizeFactor,
                float distanceFactor,
                float asteroidBeltDensity,
                float ringSizeFactor);
};

}
