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
#include <vector>

#include "generator/BezierPatch.hpp"
#include "generator/figures/Sphere.hpp"
#include "generator/figures/Torus.hpp"
#include "generator/SolarSystem.hpp"

namespace generator {

SolarSystem::SolarSystem(float sunScale, float rockyScale, float gasScale, float _timeScale) :
    rng(10), bodyScale(1.0f), timeScale(_timeScale) {

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

    BezierPatch bezierPatch("res/patches/comet.patch", 10);
    bezierPatch.writeToFile(directoryPath / "comet.3d");
}

tinyxml2::XMLElement *SolarSystem::createVector(const std::string &name, const glm::vec3 &vec) {
    tinyxml2::XMLElement *translate = this->document.NewElement(name.c_str());
    translate->SetAttribute("x", vec.x);
    translate->SetAttribute("y", vec.y);
    translate->SetAttribute("z", vec.z);
    return translate;
}

tinyxml2::XMLElement *SolarSystem::createBody(float radius,
                                              float distance,
                                              float orbitTime,
                                              float rotationTime,
                                              float y,
                                              bool hasOrbiters) {
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
    std::uniform_real_distribution fullAngleDistribution(0.0f, glm::two_pi<float>());
    std::uniform_real_distribution axisOffsetDistribution(-0.2f, 0.2f);

    // Translation
    const float translationAngle = fullAngleDistribution(this->rng);
    this->lastTranslationAngle = translationAngle;

    if (orbitTime > 0.0f && this->timeScale > 0.0f) {
        tinyxml2::XMLElement *translate = this->document.NewElement("translate");
        translate->SetAttribute("time", orbitTime * this->timeScale);
        translate->SetAttribute("align", "true");

        const int numPoints = 16;
        const float angleIncrement = glm::two_pi<float>() / numPoints;
        for (int i = 0; i < numPoints; ++i) {
            const float angle = translationAngle + i * angleIncrement;
            glm::vec3 point(distance * cosf(angle), y, distance * sinf(angle));
            translate->InsertEndChild(this->createVector("point", point));
        }

        transform->InsertEndChild(translate);
    } else {
        const glm::vec3 position(distance * cosf(translationAngle),
                                 0.0f,
                                 distance * sinf(translationAngle));

        transform->InsertEndChild(this->createVector("translate", position));
    }

    // Rotation
    if (rotationTime > 0.0f && this->timeScale > 0.0f) {
        const float rotationAngle = fullAngleDistribution(this->rng);
        const float axisOffset = axisOffsetDistribution(this->rng);
        const glm::vec3 rotationAxis(axisOffset * cosf(rotationAngle),
                                     1.0f,
                                     axisOffset * sinf(rotationAngle));

        tinyxml2::XMLElement *rotate = this->createVector("rotate", glm::normalize(rotationAxis));
        rotate->SetAttribute("time", rotationTime * this->timeScale);
        innerTransform->InsertEndChild(rotate);
    }

    // Scale
    innerTransform->InsertEndChild(
        this->createVector("scale", glm::vec3(radius * this->bodyScale)));

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

tinyxml2::XMLElement *SolarSystem::createAsteroidBelt(float minDistance,
                                                      float maxDistance,
                                                      float orbitTime,
                                                      int numAsteroids) {
    // Initialize hierarchy
    tinyxml2::XMLElement *parentGroup = this->document.NewElement("group");

    const float averageRadius = (minDistance + maxDistance) / 2.0f;
    const float beltWidth = maxDistance - minDistance;
    const int numGroups = ceilf(glm::two_pi<float>() * averageRadius / beltWidth);
    const float groupArc = glm::two_pi<float>() / numGroups;

    std::vector<tinyxml2::XMLElement *> subGroups;
    subGroups.reserve(numGroups);
    for (int i = 0; i < numGroups; ++i) {
        subGroups.push_back(parentGroup->InsertNewChildElement("group"));
    }

    // Create asteroids
    for (int i = 0; i < numAsteroids; i++) {
        std::uniform_real_distribution distanceDistribution(minDistance, maxDistance);
        std::uniform_real_distribution radiusDistribution(1.0f, 2.0f);
        std::uniform_real_distribution yDistribution(-20.0f, 20.0f);

        const float distance = distanceDistribution(this->rng);
        const float radius = radiusDistribution(this->rng);
        const float y = yDistribution(this->rng);

        tinyxml2::XMLElement *asteroid = this->createBody(radius, distance, orbitTime, 0.0f, y);
        const int group = floorf(this->lastTranslationAngle / groupArc);
        subGroups[group]->InsertEndChild(asteroid);
    }

    return parentGroup;
}

tinyxml2::XMLElement *SolarSystem::createComet() {
    tinyxml2::XMLElement *group = this->document.NewElement("group");
    tinyxml2::XMLElement *transform = group->InsertNewChildElement("transform");

    tinyxml2::XMLElement *translate = transform->InsertNewChildElement("translate");
    translate->SetAttribute("time", 100.0f);
    translate->SetAttribute("align", true);

    const std::vector<glm::vec3> points = {
        { -800.0f, 50.0f,  -400.0f },
        { -400.0f, 100.0f, 0.0f    },
        { 0.0f,    150.0f, 400.0f  },
        { 400.0f,  100.0f, 0.0f    },
        { 800.0f,  50.0f,  -400.0f }
    };

    for (const glm::vec3 &point : points) {
        translate->InsertEndChild(this->createVector("point", point));
    }
    transform->InsertEndChild(this->createVector("scale", glm::vec3(this->bodyScale)));

    tinyxml2::XMLElement *models = group->InsertNewChildElement("models");
    tinyxml2::XMLElement *model = models->InsertNewChildElement("model");
    model->SetAttribute("file", "comet.3d");
    return group;
}

void SolarSystem::createPreamble(float sunScale, float rockyScale, float gasScale) {
    const std::string description = " solarSystem " + std::to_string(sunScale) + " " +
        std::to_string(rockyScale) + " " + std::to_string(gasScale) + " " +
        std::to_string(this->timeScale);
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
    group->InsertEndChild(this->createBody(80.0f, 0.0f, 0.0f, 10.0f));

    // Rocky planets
    this->bodyScale = rockyScale;

    group->InsertEndChild(this->createBody(5.0f, 200.0f, 25.0f, 15.0f)); // Mercury
    group->InsertEndChild(this->createBody(6.5f, 370.0f, 35.0f, 18.0f)); // Venus

    tinyxml2::XMLElement *earth = this->createBody(7.0f, 550.0f, 50.0f, 20.0f, 0.0f, true);
    earth->InsertEndChild(this->createBody(1.0f, rockyScale * 12.0f, 10.0f, 10.0f)); // Moon
    group->InsertEndChild(earth);

    group->InsertEndChild(this->createBody(6.5f, 730.0f, 65.0f, 22.0f)); // Mars

    // Gas giants
    this->bodyScale = gasScale;

    tinyxml2::XMLElement *jupiter = this->createBody(35.0f, 1900.0f, 90.0f, 25.0f, 0.0f, true);

    this->bodyScale = rockyScale;
    jupiter->InsertEndChild(this->createBody(0.5f, 40.0f * gasScale, 3.0f, 5.0f)); // Io
    jupiter->InsertEndChild(this->createBody(0.5f, 45.0f * gasScale, 4.0f, 6.0f)); // Europa
    jupiter->InsertEndChild(this->createBody(0.7f, 50.0f * gasScale, 5.0f, 7.0f)); // Ganymede
    jupiter->InsertEndChild(this->createBody(0.7f, 55.0f * gasScale, 6.0f, 8.0f)); // Callisto
    group->InsertEndChild(jupiter);

    this->bodyScale = gasScale;

    tinyxml2::XMLElement *saturn = this->createBody(30.0f, 2400.0f, 100.0f, 30.0f);
    saturn->InsertEndChild(this->createRings(3.0f));
    group->InsertEndChild(saturn);

    group->InsertEndChild(this->createBody(24.0f, 2850.0f, 130.0f, 32.0f)); // Uranus
    group->InsertEndChild(this->createBody(23.0f, 3330.0f, 150.0f, 35.0f)); // Neptune

    // Asteroid belts
    this->bodyScale = rockyScale;
    group->InsertEndChild(this->createAsteroidBelt(1000.0f, 1600.0f, 80.0f, 2000));
    group->InsertEndChild(this->createAsteroidBelt(3500.0, 3800.0, 4000.0f, 2000));
    group->InsertEndChild(this->createComet());
}

}
