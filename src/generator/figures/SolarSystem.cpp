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

#include "generator/figures/SolarSystem.hpp"

namespace generator::figures {

SolarSystem::SolarSystem(float scale) {
    createWorld();
    configureCamera();
    generateSolarSystem(scale);
    writeToFile("scene_solarSystem.xml");
}

void SolarSystem::createWorld() {
    tinyxml2::XMLElement *world = doc.NewElement("world");
    doc.InsertFirstChild(world);

    tinyxml2::XMLElement *window = doc.NewElement("window");
    window->SetAttribute("width", 1024);
    window->SetAttribute("height", 768);
    world->InsertEndChild(window);
}

void SolarSystem::configureCamera() {
    tinyxml2::XMLElement *camera = doc.NewElement("camera");
    camera->SetAttribute("type", "free");
    doc.FirstChildElement("world")->InsertEndChild(camera);

    struct {
        const char *name;
        float x, y, z;
    } cameraSettings[] = {
        { "position", 120, 90, 150 },
        { "lookAt",   0,   0,  0   },
        { "up",       0,   1,  0   }
    };

    for (const auto &setting : cameraSettings) {
        tinyxml2::XMLElement *element = doc.NewElement(setting.name);
        element->SetAttribute("x", setting.x);
        element->SetAttribute("y", setting.y);
        element->SetAttribute("z", setting.z);
        camera->InsertEndChild(element);
    }

    tinyxml2::XMLElement *projection = doc.NewElement("projection");
    projection->SetAttribute("fov", 75);
    projection->SetAttribute("near", 1);
    projection->SetAttribute("far", 6000);
    camera->InsertEndChild(projection);
}

void SolarSystem::generateSolarSystem(float scale) {
    tinyxml2::XMLElement *solarSystem = doc.NewElement("group");
    doc.FirstChildElement("world")->InsertEndChild(solarSystem);

    addPlanet(solarSystem, 0, 0, 0, 50 * scale); // Sun
    addPlanet(solarSystem, 150 * scale, 6, -15, 0.4 * scale); // Mercury
    addPlanet(solarSystem, 280 * scale, -3, 9, 0.5 * scale); // Venus

    tinyxml2::XMLElement *earth = addPlanet(solarSystem, 400 * scale, 0, -18, 0.6 * scale); // Earth
    addSatellite(earth, 2 * scale, 1.2 * scale, 1.5, 0.15 * scale); // Moon

    tinyxml2::XMLElement *mars = addPlanet(solarSystem, 600 * scale, 3, 24, 0.5 * scale); // Mars
    addSatellite(mars, 1.5 * scale, 0.6 * scale, 0.8, 0.1 * scale); // Phobos
    addSatellite(mars, -1.2 * scale, -0.5 * scale, 0.5, 0.08 * scale); // Deimos

    tinyxml2::XMLElement *jupiter =
        addPlanet(solarSystem, 1200 * scale, -6, -36, 5 * scale); // Jupiter
    addSatellite(jupiter, 20 * scale, 8 * scale, 10, 0.6 * scale); // Ganymede
    addSatellite(jupiter, -18 * scale, 6 * scale, -12, 0.5 * scale); // Callisto
    addSatellite(jupiter, 16 * scale, -4 * scale, 8, 0.4 * scale); // Io
    addSatellite(jupiter, -14 * scale, 3 * scale, -6, 0.3 * scale); // Europa

    tinyxml2::XMLElement *saturn =
        addPlanet(solarSystem, 1800 * scale, 6, 30, 4.5 * scale); // Saturn
    addRing(saturn, 1.2, 0.1, 1.2, 15); // Saturn’s ring inclination
    addSatellite(saturn, 30 * scale, 12 * scale, 15, 0.4 * scale); // Titan
    addSatellite(saturn, -24 * scale, 9 * scale, -18, 0.3 * scale); // Enceladus

    tinyxml2::XMLElement *uranus =
        addPlanet(solarSystem, 2300 * scale, -9, -45, 2 * scale); // Uranus
    addRing(uranus, 1.1, 0.1, 1.1, 90); // Uranus’ vertical ring
    addSatellite(uranus, 14 * scale, 5 * scale, 10, 0.2 * scale); // Miranda
    addSatellite(uranus, -12 * scale, 4 * scale, -8, 0.15 * scale); // Ariel

    tinyxml2::XMLElement *neptune =
        addPlanet(solarSystem, 2800 * scale, 12, 60, 2 * scale); // Neptune
    addRing(neptune, 1.05, 0.06, 1.05, 30); // Neptune’s ring inclination
    addSatellite(neptune, 16 * scale, 6 * scale, 12, 0.3 * scale); // Triton

    addAsteroidBelt(solarSystem, 650 * scale, 1000 * scale, 400); // Asteroid Belt
    addAsteroidBelt(solarSystem, 3100 * scale, 4000 * scale, 1200); // Kuiper Belt
}

tinyxml2::XMLElement *
    SolarSystem::addPlanet(tinyxml2::XMLElement *parent, float x, float y, float z, float size) {
    tinyxml2::XMLElement *group = doc.NewElement("group");
    parent->InsertEndChild(group);

    tinyxml2::XMLElement *transform = doc.NewElement("transform");
    group->InsertEndChild(transform);

    tinyxml2::XMLElement *translate = doc.NewElement("translate");
    translate->SetAttribute("x", x);
    translate->SetAttribute("y", y);
    translate->SetAttribute("z", z);
    transform->InsertEndChild(translate);

    tinyxml2::XMLElement *scaleElem = doc.NewElement("scale");
    scaleElem->SetAttribute("x", size);
    scaleElem->SetAttribute("y", size);
    scaleElem->SetAttribute("z", size);
    transform->InsertEndChild(scaleElem);

    tinyxml2::XMLElement *models = doc.NewElement("models");
    group->InsertEndChild(models);

    tinyxml2::XMLElement *model = doc.NewElement("model");
    model->SetAttribute("file", "../models/sphere.3d");
    models->InsertEndChild(model);

    return group;
}

void
    SolarSystem::addSatellite(tinyxml2::XMLElement *parent, float x, float y, float z, float size) {
    addPlanet(parent, x, y, z, size);
}

void SolarSystem::addRing(tinyxml2::XMLElement *parent,
                          float scaleX,
                          float scaleY,
                          float scaleZ,
                          float angle) {
    tinyxml2::XMLElement *group = doc.NewElement("group");
    parent->InsertEndChild(group);

    tinyxml2::XMLElement *transform = doc.NewElement("transform");
    group->InsertEndChild(transform);

    tinyxml2::XMLElement *rotate = doc.NewElement("rotate");
    rotate->SetAttribute("angle", angle);
    rotate->SetAttribute("x", 1);
    rotate->SetAttribute("y", 0);
    rotate->SetAttribute("z", 0);
    transform->InsertEndChild(rotate);

    tinyxml2::XMLElement *scaleElem = doc.NewElement("scale");
    scaleElem->SetAttribute("x", scaleX);
    scaleElem->SetAttribute("y", scaleY);
    scaleElem->SetAttribute("z", scaleZ);
    transform->InsertEndChild(scaleElem);

    tinyxml2::XMLElement *models = doc.NewElement("models");
    group->InsertEndChild(models);

    tinyxml2::XMLElement *model = doc.NewElement("model");
    model->SetAttribute("file", "../models/torus.3d");
    models->InsertEndChild(model);
}

void SolarSystem::addAsteroidBelt(tinyxml2::XMLElement *parent,
                                  float minDist,
                                  float maxDist,
                                  int numAsteroids) {
    tinyxml2::XMLElement *asteroidBelt = doc.NewElement("group");
    parent->InsertEndChild(asteroidBelt);

    for (int i = 0; i < numAsteroids; i++) {
        float distance = minDist + static_cast<float>(rand()) / (RAND_MAX / (maxDist - minDist));
        float angle = static_cast<float>(rand()) / (RAND_MAX / 360.0f);
        float x = distance * cos(angle);
        float z = distance * sin(angle);
        float y = static_cast<float>(rand()) / (RAND_MAX / 10.0f) - 5.0f;
        float size = 0.2f + static_cast<float>(rand()) / (RAND_MAX / 0.3f);

        addPlanet(asteroidBelt, x, y, z, size);
    }
}

void SolarSystem::writeToFile(const std::string &filename) {
    std::string filePath = "res/scenes/" + filename;
    doc.SaveFile(filePath.c_str());
}

}
