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

#include <vector>

#include "generator/figures/SolarSystem.hpp"

namespace generator::figures {

SolarSystem::SolarSystem(float sceneScale,
                         float sunSizeFactor,
                         float planetSizeFactor,
                         float moonSizeFactor,
                         float distanceFactor,
                         float asteroidBeltDensity,
                         float ringSizeFactor) {
    createWorld();
    configureCamera();
    generateSolarSystem(sceneScale,
                        sunSizeFactor,
                        planetSizeFactor,
                        moonSizeFactor,
                        distanceFactor,
                        asteroidBeltDensity,
                        ringSizeFactor);
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

    const struct {
        const char *name;
        float x, y, z;
    } cameraSettings[] = {
        { "position", 200, 150, 300 },
        { "lookAt",   0,   0,   0   },
        { "up",       0,   1,   0   }
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

void SolarSystem::generateSolarSystem(float sceneScale,
                                      float sunSizeFactor,
                                      float planetSizeFactor,
                                      float moonSizeFactor,
                                      float distanceFactor,
                                      float asteroidBeltDensity,
                                      float ringSizeFactor) {
    tinyxml2::XMLElement *world = doc.FirstChildElement("world");
    tinyxml2::XMLElement *solarSystem = doc.NewElement("group");
    world->InsertEndChild(solarSystem);

    tinyxml2::XMLElement *sun = addCelestialBody(solarSystem,
                                                 0,
                                                 0,
                                                 0,
                                                 30 * sceneScale * sunSizeFactor,
                                                 "../models/sphere.3d",
                                                 0,
                                                 0,
                                                 0,
                                                 0);

    addCelestialBody(sun,
                     12 * sceneScale * distanceFactor,
                     2 * distanceFactor,
                     -3 * distanceFactor,
                     0.15 * sceneScale * planetSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);

    addCelestialBody(sun,
                     20 * sceneScale * distanceFactor,
                     -1.5 * distanceFactor,
                     2 * distanceFactor,
                     0.18 * sceneScale * planetSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);

    tinyxml2::XMLElement *earth = addCelestialBody(sun,
                                                   28 * sceneScale * distanceFactor,
                                                   0 * distanceFactor,
                                                   -3 * distanceFactor,
                                                   0.2 * sceneScale * planetSizeFactor,
                                                   "../models/sphere.3d",
                                                   0,
                                                   0,
                                                   0,
                                                   0);
    addCelestialBody(earth,
                     3 * sceneScale,
                     0,
                     0,
                     0.05 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);

    tinyxml2::XMLElement *mars = addCelestialBody(sun,
                                                  36 * sceneScale * distanceFactor,
                                                  1.5 * distanceFactor,
                                                  5 * distanceFactor,
                                                  0.15 * sceneScale * planetSizeFactor,
                                                  "../models/sphere.3d",
                                                  0,
                                                  0,
                                                  0,
                                                  0);
    addCelestialBody(mars,
                     2 * sceneScale,
                     0.5,
                     0,
                     0.04 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);
    addCelestialBody(mars,
                     -1.5 * sceneScale,
                     -0.3,
                     1,
                     0.03 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);

    tinyxml2::XMLElement *jupiter = addCelestialBody(sun,
                                                     56 * sceneScale * distanceFactor,
                                                     -2 * distanceFactor,
                                                     -6 * distanceFactor,
                                                     0.5 * sceneScale * planetSizeFactor,
                                                     "../models/sphere.3d",
                                                     0,
                                                     0,
                                                     0,
                                                     0);
    addCelestialBody(jupiter,
                     5 * sceneScale,
                     3 * sceneScale,
                     4,
                     0.1 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);
    addCelestialBody(jupiter,
                     -4 * sceneScale,
                     2.5 * sceneScale,
                     -5,
                     0.3 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);
    addCelestialBody(jupiter,
                     3 * sceneScale,
                     -2 * sceneScale,
                     3,
                     0.3 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);
    addCelestialBody(jupiter,
                     -3 * sceneScale,
                     1 * sceneScale,
                     -2,
                     0.1 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);

    tinyxml2::XMLElement *saturn = addCelestialBody(sun,
                                                    74 * sceneScale * distanceFactor,
                                                    2 * distanceFactor,
                                                    -10 * distanceFactor,
                                                    0.4 * sceneScale * planetSizeFactor,
                                                    "../models/sphere.3d",
                                                    0,
                                                    0,
                                                    0,
                                                    0);
    addCelestialBody(saturn,
                     0,
                     0,
                     0,
                     0.6 * sceneScale * ringSizeFactor,
                     "../models/torus.3d",
                     26.7,
                     0,
                     0,
                     1);
    addCelestialBody(saturn,
                     10 * sceneScale,
                     5 * sceneScale,
                     7,
                     0.15 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);
    addCelestialBody(saturn,
                     -8 * sceneScale,
                     4 * sceneScale,
                     -8,
                     0.12 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);

    tinyxml2::XMLElement *uranus = addCelestialBody(sun,
                                                    92 * sceneScale * distanceFactor,
                                                    -4 * distanceFactor,
                                                    -12 * distanceFactor,
                                                    0.3 * sceneScale * planetSizeFactor,
                                                    "../models/sphere.3d",
                                                    0,
                                                    0,
                                                    0,
                                                    0);
    addCelestialBody(uranus,
                     0,
                     0,
                     0,
                     0.5 * sceneScale * ringSizeFactor,
                     "../models/torus.3d",
                     90,
                     1,
                     0,
                     0);
    addCelestialBody(uranus,
                     4 * sceneScale,
                     2 * sceneScale,
                     3,
                     0.05 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);
    addCelestialBody(uranus,
                     -3.5 * sceneScale,
                     1.5 * sceneScale,
                     -3,
                     0.04 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);

    tinyxml2::XMLElement *neptune = addCelestialBody(sun,
                                                     110 * sceneScale * distanceFactor,
                                                     5 * distanceFactor,
                                                     -15 * distanceFactor,
                                                     0.3 * sceneScale * planetSizeFactor,
                                                     "../models/sphere.3d",
                                                     0,
                                                     0,
                                                     0,
                                                     0);
    addCelestialBody(neptune,
                     0,
                     0,
                     0,
                     0.45 * sceneScale * ringSizeFactor,
                     "../models/torus.3d",
                     28.3,
                     0,
                     0,
                     1);
    addCelestialBody(neptune,
                     5 * sceneScale,
                     2.5 * sceneScale,
                     4,
                     0.05 * sceneScale * moonSizeFactor,
                     "../models/sphere.3d",
                     0,
                     0,
                     0,
                     0);

    addAsteroidBelt(sun,
                    40 * sceneScale * distanceFactor,
                    49 * sceneScale * distanceFactor,
                    600 * asteroidBeltDensity,
                    sceneScale);
    addAsteroidBelt(sun,
                    120 * sceneScale * distanceFactor,
                    150 * sceneScale * distanceFactor,
                    1400 * asteroidBeltDensity,
                    sceneScale);
}

tinyxml2::XMLElement *SolarSystem::addCelestialBody(tinyxml2::XMLElement *parent,
                                                    float x,
                                                    float y,
                                                    float z,
                                                    float size,
                                                    const std::string &modelFile,
                                                    float angle,
                                                    float rotX,
                                                    float rotY,
                                                    float rotZ) {

    tinyxml2::XMLElement *group = doc.NewElement("group");
    parent->InsertEndChild(group);

    tinyxml2::XMLElement *transform = doc.NewElement("transform");
    group->InsertEndChild(transform);

    tinyxml2::XMLElement *translate = doc.NewElement("translate");
    translate->SetAttribute("x", x);
    translate->SetAttribute("y", y);
    translate->SetAttribute("z", z);
    transform->InsertEndChild(translate);

    if (angle != 0) {
        tinyxml2::XMLElement *rotate = doc.NewElement("rotate");
        rotate->SetAttribute("angle", angle);
        rotate->SetAttribute("x", rotX);
        rotate->SetAttribute("y", rotY);
        rotate->SetAttribute("z", rotZ);
        transform->InsertEndChild(rotate);
    }

    tinyxml2::XMLElement *scaleElem = doc.NewElement("scale");
    if (modelFile == "../models/torus.3d") {
        scaleElem->SetAttribute("x", size);
        scaleElem->SetAttribute("y", size * 0.1);
        scaleElem->SetAttribute("z", size);
    } else {
        scaleElem->SetAttribute("x", size);
        scaleElem->SetAttribute("y", size);
        scaleElem->SetAttribute("z", size);
    }
    transform->InsertEndChild(scaleElem);

    tinyxml2::XMLElement *models = doc.NewElement("models");
    group->InsertEndChild(models);

    tinyxml2::XMLElement *model = doc.NewElement("model");
    model->SetAttribute("file", modelFile.c_str());
    models->InsertEndChild(model);

    return group;
}

void SolarSystem::addAsteroidBelt(tinyxml2::XMLElement *parent,
                                  float minDist,
                                  float maxDist,
                                  int numAsteroids,
                                  float sceneScale) {
    tinyxml2::XMLElement *asteroidBelt = doc.NewElement("group");
    parent->InsertEndChild(asteroidBelt);

    std::vector<std::string> models = { "../models/sphere.3d",
                                        "../models/box.3d",
                                        "../models/cylinder.3d" };

    for (int i = 0; i < numAsteroids; i++) {
        float distance = minDist + static_cast<float>(rand()) / (RAND_MAX / (maxDist - minDist));
        float angle = static_cast<float>(rand()) / (RAND_MAX / 360.0f);
        float x = distance * cos(angle);
        float z = distance * sin(angle);
        float y = static_cast<float>(rand()) / (RAND_MAX / 10.0f) - 5.0f;

        float size = (0.01 + (static_cast<float>(rand()) / RAND_MAX) * (0.06 - 0.01)) * sceneScale;

        std::string modelFile = models[rand() % models.size()];

        addCelestialBody(asteroidBelt,
                         x,
                         y,
                         z,
                         size,
                         modelFile,
                         static_cast<float>(rand()) / (RAND_MAX / 360.0f),
                         static_cast<float>(rand()) / RAND_MAX,
                         static_cast<float>(rand()) / RAND_MAX,
                         static_cast<float>(rand()) / RAND_MAX);
    }
}

void SolarSystem::writeToFile(const std::string &filename) {
    std::string filePath = "res/scenes/" + filename;
    doc.SaveFile(filePath.c_str());
}

}
