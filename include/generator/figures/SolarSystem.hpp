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

#include <string>
#include <tinyxml2.h>
#include <math.h>

namespace generator::figures {

class SolarSystem {
private:
    tinyxml2::XMLDocument doc;

    void createWorld();
    void configureCamera();
    void generateSolarSystem(float scale);

    tinyxml2::XMLElement* addPlanet(tinyxml2::XMLElement *parent, float x, float y, float z, float size);
    void addSatellite(tinyxml2::XMLElement *parent, float x, float y, float z, float size);
    void addRing(tinyxml2::XMLElement *parent, float scaleX, float scaleY, float scaleZ, float angle);
    void addAsteroidBelt(tinyxml2::XMLElement *parent, float minDist, float maxDist, int numAsteroids);

public:
    SolarSystem(float scale);
    void writeToFile(const std::string &filename);
};

}
