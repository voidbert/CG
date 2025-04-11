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
#include <filesystem>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

#include "generator/figures/Sphere.hpp"
#include "generator/figures/Torus.hpp"
#include "generator/SolarSystem.hpp"

namespace generator {

SolarSystem::SolarSystem(float sunScale, float rockyScale, float gasScale) :
    rng(10), bodyScale(1.0f) {

    this->createPreamble(sunScale, rockyScale, gasScale);
    this->createCamera();
    this->createObjects(sunScale, rockyScale, gasScale);
}

void SolarSystem::writeToFile(const std::string &dirname) {
    std::filesystem::create_directory(dirname);
    const std::filesystem::path directoryPath = dirname;

    const std::string sceneFile = directoryPath / "scene.xml";
    const int res = this->document.SaveFile(sceneFile.c_str());
    if (res != tinyxml2::XML_SUCCESS) {
        throw std::ios_base::failure("Error while writing XML file: " + sceneFile);
    }

    figures::Sphere sphere(1.0f, 32, 32);
    sphere.writeToFile(directoryPath / "sphere.3d");

    figures::Torus torus(1.0f, 0.2f, 32, 8);
    torus.writeToFile(directoryPath / "torus.3d");
}

tinyxml2::XMLElement *SolarSystem::createVector(const std::string &name, const glm::vec3 &vec) {
    tinyxml2::XMLElement *translate = this->document.NewElement(name.c_str());
    translate->SetAttribute("x", vec.x);
    translate->SetAttribute("y", vec.y);
    translate->SetAttribute("z", vec.z);
    return translate;
}

tinyxml2::XMLElement *
    SolarSystem::createBody(float radius, float distance, bool hasOrbiters, float y) {

    // Setup hierarchy
    tinyxml2::XMLElement *group = this->document.NewElement("group");
    tinyxml2::XMLElement *transform = group->InsertNewChildElement("transform");

    tinyxml2::XMLElement *innerGroup = group, *innerTransform = transform;
    if (hasOrbiters) {
        innerGroup = group->InsertNewChildElement("group");
        innerTransform = innerGroup->InsertNewChildElement("transform");
    }

    // Model
    tinyxml2::XMLElement *models = innerGroup->InsertNewChildElement("models");
    tinyxml2::XMLElement *model = models->InsertNewChildElement("model");
    model->SetAttribute("file", "sphere.3d");

    // Distributions for transforms
    std::uniform_real_distribution fullAngleDistribution(0.0f, 2.0f * glm::pi<float>());
    std::uniform_real_distribution axisOffsetDistribution(-0.5f, 0.5f);

    // Translation
    const float translationAngle = fullAngleDistribution(this->rng);
    const glm::vec3 position(distance * cosf(translationAngle),
                             y,
                             distance * sinf(translationAngle));

    transform->InsertEndChild(this->createVector("translate", position));

    // Scale
    innerTransform->InsertEndChild(
        this->createVector("scale", glm::vec3(radius * this->bodyScale)));

    // Rotation
    const float rotationAngle = fullAngleDistribution(this->rng);
    const float axisOffset = distance == 0.0f ? 0.0f : axisOffsetDistribution(this->rng);
    const glm::vec3 rotationAxis(axisOffset * cosf(rotationAngle),
                                 1.0f,
                                 axisOffset * sinf(rotationAngle));

    tinyxml2::XMLElement *rotate = this->createVector("rotate", glm::normalize(rotationAxis));
    rotate->SetAttribute("angle", rotationAngle);
    innerTransform->InsertEndChild(rotate);

    return group;
}

tinyxml2::XMLElement *SolarSystem::createRings(float radius) {
    tinyxml2::XMLElement *group = this->document.NewElement("group");
    tinyxml2::XMLElement *transform = group->InsertNewChildElement("transform");

    // Model
    tinyxml2::XMLElement *models = group->InsertNewChildElement("models");
    tinyxml2::XMLElement *model = models->InsertNewChildElement("model");
    model->SetAttribute("file", "torus.3d");

    // Scale
    transform->InsertEndChild(
        this->createVector("scale", glm::vec3(radius, 0.1f * radius, radius)));

    return group;
}

tinyxml2::XMLElement *
    SolarSystem::createAsteroidBelt(float minDistance, float maxDistance, int numAsteroids) {

    tinyxml2::XMLElement *group = this->document.NewElement("group");

    for (int i = 0; i < numAsteroids; i++) {
        std::uniform_real_distribution distanceDistribution(minDistance, maxDistance);
        std::uniform_real_distribution radiusDistribution(0.02f, 0.10f);
        std::uniform_real_distribution yDistribution(-10.0f, 10.0f);

        const float distance = distanceDistribution(this->rng);
        const float radius = radiusDistribution(this->rng);
        const float y = yDistribution(this->rng);

        group->InsertEndChild(this->createBody(radius, distance, false, y));
    }

    return group;
}

void SolarSystem::createPreamble(float sunScale, float rockyScale, float gasScale) {
    const std::string description = " solarSystem " + std::to_string(sunScale) + " " +
        std::to_string(rockyScale) + " " + std::to_string(gasScale) + " ";
    tinyxml2::XMLComment *comment = this->document.NewComment(description.c_str());
    this->document.InsertEndChild(comment);

    this->world = this->document.NewElement("world");
    this->document.InsertEndChild(this->world);

    tinyxml2::XMLElement *window = this->world->InsertNewChildElement("window");
    window->SetAttribute("width", 1024);
    window->SetAttribute("height", 768);
}

void SolarSystem::createCamera() {
    tinyxml2::XMLElement *camera = this->world->InsertNewChildElement("camera");
    camera->SetAttribute("type", "free");

    camera->InsertEndChild(this->createVector("position", glm::vec3(100, 100, 100)));
    camera->InsertEndChild(this->createVector("lookAt", glm::vec3(0, 0, 0)));
    camera->InsertEndChild(this->createVector("up", glm::vec3(0, 1, 0)));

    tinyxml2::XMLElement *projection = camera->InsertNewChildElement("projection");
    projection->SetAttribute("fov", 60);
    projection->SetAttribute("near", 1);
    projection->SetAttribute("far", 6000);
}

void SolarSystem::createObjects(float sunScale, float rockyScale, float gasScale) {
    tinyxml2::XMLElement *group = this->world->InsertNewChildElement("group");

    // Sun
    this->bodyScale = sunScale;
    group->InsertEndChild(this->createBody(30.0f, 0.0f));

    // Rocky planets
    this->bodyScale = rockyScale;

    group->InsertEndChild(this->createBody(0.2f, 50.0f)); // Mercury
    group->InsertEndChild(this->createBody(0.4f, 100.0f)); // Venus

    tinyxml2::XMLElement *earth = this->createBody(0.4f, 150.0f, true);
    earth->InsertEndChild(this->createBody(0.15f, rockyScale)); // Moon
    group->InsertEndChild(earth);

    group->InsertEndChild(this->createBody(0.25f, 250.0f)); // Mars

    // Gas giants
    this->bodyScale = gasScale;

    tinyxml2::XMLElement *jupiter = this->createBody(3.0f, 500.0f, true);

    this->bodyScale = rockyScale;
    jupiter->InsertEndChild(this->createBody(0.1f, 4.0f * gasScale)); // Io
    jupiter->InsertEndChild(this->createBody(0.1f, 4.5f * gasScale)); // Europa
    jupiter->InsertEndChild(this->createBody(0.2f, 5.0f * gasScale)); // Ganymede
    jupiter->InsertEndChild(this->createBody(0.2f, 5.0f * gasScale)); // Callisto
    group->InsertEndChild(jupiter);

    this->bodyScale = gasScale;

    tinyxml2::XMLElement *saturn = this->createBody(2.5f, 750.0f);
    saturn->InsertEndChild(this->createRings(1.5f));
    group->InsertEndChild(saturn);

    group->InsertEndChild(this->createBody(1.0f, 1000.0f)); // Uranus
    group->InsertEndChild(this->createBody(1.0f, 1250.0f)); // Neptune

    // Asteroid belts
    this->bodyScale = rockyScale;
    group->InsertEndChild(this->createAsteroidBelt(300.0f, 450.0f, 600));
    group->InsertEndChild(this->createAsteroidBelt(1400.0, 1800.0, 2000));
}

}
