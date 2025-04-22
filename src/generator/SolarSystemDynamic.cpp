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
#include "generator/SolarSystemDynamic.hpp"

namespace generator {

SolarSystemDynamic::SolarSystemDynamic(float sunScale,
                                       float rockyScale,
                                       float gasScale,
                                       float timeScale) :
    rng(10), bodyScale(1.0f) {

    this->createPreamble(sunScale, rockyScale, gasScale, timeScale);
    this->createCamera();
    this->createObjects(sunScale, rockyScale, gasScale, timeScale);
}

void SolarSystemDynamic::writeToFile(const std::string &dirname) {
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

    const std::string patchFile = "res/patches/comet.patch";
    BezierPatch bezierPatch(patchFile, 10);
    bezierPatch.writeToFile(directoryPath / "comet.3d");
}

tinyxml2::XMLElement *SolarSystemDynamic::createVector(const std::string &name,
                                                       const glm::vec3 &vec) {
    tinyxml2::XMLElement *translate = this->document.NewElement(name.c_str());
    translate->SetAttribute("x", vec.x);
    translate->SetAttribute("y", vec.y);
    translate->SetAttribute("z", vec.z);
    return translate;
}

tinyxml2::XMLElement *SolarSystemDynamic::createBody(float radius,
                                                     float distance,
                                                     float orbitTime,
                                                     float rotationTime,
                                                     bool hasOrbiters,
                                                     float y) {

    tinyxml2::XMLElement *group = this->document.NewElement("group");
    tinyxml2::XMLElement *transform = group->InsertNewChildElement("transform");

    tinyxml2::XMLElement *innerGroup = group, *innerTransform = transform;
    if (hasOrbiters) {
        innerGroup = group->InsertNewChildElement("group");
        innerTransform = innerGroup->InsertNewChildElement("transform");
    }

    tinyxml2::XMLElement *models = innerGroup->InsertNewChildElement("models");
    tinyxml2::XMLElement *model = models->InsertNewChildElement("model");
    model->SetAttribute("file", "sphere.3d");

    std::uniform_real_distribution fullAngleDistribution(0.0f, 2.0f * glm::pi<float>());
    std::uniform_real_distribution axisOffsetDistribution(-0.2f, 0.2f);

    const float translationAngle = fullAngleDistribution(this->rng);
    this->lastTranslationAngle = translationAngle;

    if (distance > 0.0f && orbitTime > 0.0f) {
        tinyxml2::XMLElement *translate = this->document.NewElement("translate");
        translate->SetAttribute("time", orbitTime);
        translate->SetAttribute("align", "true");

        const int numPoints = 16;
        std::uniform_real_distribution<float> startAngleDist(0.0f, 2.0f * glm::pi<float>());
        float startAngle = startAngleDist(this->rng);

        for (int i = 0; i < numPoints; ++i) {
            float angle = startAngle + 2.0f * glm::pi<float>() * i / numPoints;
            glm::vec3 point(distance * cos(angle), y, distance * sin(angle));
            translate->InsertEndChild(this->createVector("point", point));
        }

        transform->InsertEndChild(translate);
    } else {
        transform->InsertEndChild(this->createVector("translate", glm::vec3(distance, y, 0.0f)));
    }

    innerTransform->InsertEndChild(
        this->createVector("scale", glm::vec3(radius * this->bodyScale)));

    if (rotationTime > 0.0f) {
        tinyxml2::XMLElement *rotate = this->document.NewElement("rotate");
        rotate->SetAttribute("time", rotationTime);
        rotate->SetAttribute("x", 0.0f);
        rotate->SetAttribute("y", 1.0f);
        rotate->SetAttribute("z", 0.0f);
        innerTransform->InsertEndChild(rotate);
    }

    return group;
}

tinyxml2::XMLElement *SolarSystemDynamic::createRings(float radius) {
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
    SolarSystemDynamic::createAsteroidBelt(float minDistance, float maxDistance, int numAsteroids) {
    // Initialize hierarchy
    tinyxml2::XMLElement *parentGroup = this->document.NewElement("group");

    const float averageRadius = (minDistance + maxDistance) / 2.0f;
    const float beltWidth = maxDistance - minDistance;
    const int numGroups = ceilf(2 * glm::pi<float>() * averageRadius / beltWidth);
    const float groupArc = 2 * glm::pi<float>() / numGroups;

    std::vector<tinyxml2::XMLElement *> subGroups;
    subGroups.reserve(numGroups);
    for (int i = 0; i < numGroups; ++i) {
        subGroups.push_back(parentGroup->InsertNewChildElement("group"));
    }

    // Create asteroids
    for (int i = 0; i < numAsteroids; i++) {
        std::uniform_real_distribution distanceDistribution(minDistance, maxDistance);
        std::uniform_real_distribution radiusDistribution(0.02f, 0.10f);
        std::uniform_real_distribution yDistribution(-5.0f, 5.0f);
        std::uniform_real_distribution angleDistribution(0.0f, 2.0f * glm::pi<float>());

        const float angle = angleDistribution(this->rng);
        const float distance = distanceDistribution(this->rng);
        const float radius = radiusDistribution(this->rng);
        const float y = yDistribution(this->rng);

        const glm::vec3 position(distance * cosf(angle), y, distance * sinf(angle));

        tinyxml2::XMLElement *group = this->document.NewElement("group");
        tinyxml2::XMLElement *transform = group->InsertNewChildElement("transform");

        // Add dynamic translate
        tinyxml2::XMLElement *translate = this->document.NewElement("translate");
        translate->SetAttribute("time", 100.0f);
        translate->SetAttribute("align", "true");

        const int numPoints = 16;
        for (int j = 0; j < numPoints; ++j) {
            float pointAngle = angle + 2.0f * glm::pi<float>() * j / numPoints;
            glm::vec3 point(distance * cos(pointAngle), y, distance * sin(pointAngle));
            translate->InsertEndChild(this->createVector("point", point));
        }

        transform->InsertEndChild(translate);

        // Add scale
        transform->InsertEndChild(this->createVector("scale", glm::vec3(radius)));

        // Add model
        tinyxml2::XMLElement *models = group->InsertNewChildElement("models");
        tinyxml2::XMLElement *model = models->InsertNewChildElement("model");
        model->SetAttribute("file", "sphere.3d");

        const int subgroup = floorf(angle / groupArc);
        subGroups[subgroup]->InsertEndChild(group);
    }

    return parentGroup;
}

void SolarSystemDynamic::createPreamble(float sunScale,
                                        float rockyScale,
                                        float gasScale,
                                        float timeScale) {
    const std::string description = " solarSystem " + std::to_string(sunScale) + " " +
        std::to_string(rockyScale) + " " + std::to_string(gasScale) + " " + " " +
        std::to_string(timeScale);
    tinyxml2::XMLComment *comment = this->document.NewComment(description.c_str());
    this->document.InsertEndChild(comment);

    this->world = this->document.NewElement("world");
    this->document.InsertEndChild(this->world);

    tinyxml2::XMLElement *window = this->world->InsertNewChildElement("window");
    window->SetAttribute("width", 1024);
    window->SetAttribute("height", 768);
}

void SolarSystemDynamic::createCamera() {
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

void SolarSystemDynamic::createObjects(float sunScale,
                                       float rockyScale,
                                       float gasScale,
                                       float timeScale) {
    tinyxml2::XMLElement *group = this->world->InsertNewChildElement("group");

    // Sun
    this->bodyScale = sunScale;
    group->InsertEndChild(this->createBody(80.0f, 0.0f, 0.0f * timeScale, 10.0f * timeScale));

    // Rocky planets
    this->bodyScale = rockyScale;
    // Mercury
    group->InsertEndChild(this->createBody(5.0f, 200.0f, 25.0f * timeScale, 15.0f * timeScale));
    // Venus
    group->InsertEndChild(this->createBody(6.5f, 370.0f, 35.0f * timeScale, 18.0f * timeScale));

    // Earth
    tinyxml2::XMLElement *earth =
        this->createBody(7.0f, 550.0f, 50.0f * timeScale, 20.0f * timeScale);
    // Moon
    earth->InsertEndChild(this->createBody(0.5f, 12.0f, 10.0f * timeScale, 10.0f * timeScale));
    group->InsertEndChild(earth);

    // Mars
    group->InsertEndChild(this->createBody(6.5f, 730.0f, 65.0f * timeScale, 22.0f * timeScale));

    // Asteroids belt
    this->bodyScale = rockyScale;
    group->InsertEndChild(this->createAsteroidBelt(900.0f, 1000.0f, 600));

    // Jupiter
    this->bodyScale = gasScale;
    tinyxml2::XMLElement *jupiter =
        this->createBody(35.0f, 1900.0f, 90.0f * timeScale, 25.0f * timeScale);
    this->bodyScale = rockyScale;
    jupiter->InsertEndChild(this->createBody(0.5f, 5.0f, 3.0f * timeScale, 5.0f * timeScale)); // Io
    jupiter->InsertEndChild(
        this->createBody(0.5f, 9.0f, 4.0f * timeScale, 6.0f * timeScale)); // Europe
    jupiter->InsertEndChild(
        this->createBody(0.7f, 12.0f, 5.0f * timeScale, 7.0f * timeScale)); // Ganymede
    jupiter->InsertEndChild(
        this->createBody(0.7f, 15.0f, 6.0f * timeScale, 8.0f * timeScale)); // Callisto
    group->InsertEndChild(jupiter);

    // Saturn
    this->bodyScale = gasScale;
    tinyxml2::XMLElement *saturn =
        this->createBody(30.0f, 2400.0f, 110.0f * timeScale, 30.0f * timeScale);
    saturn->InsertEndChild(this->createRings(5.0f));
    group->InsertEndChild(saturn);

    // Uranus
    group->InsertEndChild(this->createBody(24.0f, 2850.0f, 130.0f * timeScale, 32.0f * timeScale));

    // Neptune
    group->InsertEndChild(this->createBody(23.0f, 3300.0f, 150.0f * timeScale, 35.0f * timeScale));

    // Asteroids belt
    this->bodyScale = rockyScale;
    group->InsertEndChild(this->createAsteroidBelt(4000.0f, 4500.0f, 800));

    // Comet
    group->InsertEndChild(this->createComet(rockyScale));
}

tinyxml2::XMLElement *SolarSystemDynamic::createComet(float rockyScale) {
    tinyxml2::XMLElement *group = this->document.NewElement("group");
    tinyxml2::XMLElement *transform = group->InsertNewChildElement("transform");

    tinyxml2::XMLElement *translate = this->document.NewElement("translate");
    translate->SetAttribute("time", 100.0f);
    translate->SetAttribute("align", "true");

    std::vector<glm::vec3> points = {
        { -800.0f, 50.0f,  -400.0f },
        { -400.0f, 100.0f, 0.0f    },
        { 0.0f,    150.0f, 400.0f  },
        { 400.0f,  100.0f, 0.0f    },
        { 800.0f,  50.0f,  -400.0f }
    };

    for (const auto &p : points) {
        translate->InsertEndChild(this->createVector("point", p));
    }

    transform->InsertEndChild(translate);

    // Distributions for transforms
    std::uniform_real_distribution fullAngleDistribution(0.0f, 2.0f * glm::pi<float>());
    std::uniform_real_distribution axisOffsetDistribution(-0.2f, 0.2f);

    const float rotationAngle = fullAngleDistribution(this->rng);
    const float axisOffset = axisOffsetDistribution(this->rng);
    const glm::vec3 rotationAxis(axisOffset * cosf(rotationAngle),
                                 1.0f,
                                 axisOffset * sinf(rotationAngle));

    tinyxml2::XMLElement *rotate = this->createVector("rotate", glm::normalize(rotationAxis));
    rotate->SetAttribute("angle", rotationAngle * 180.0 / glm::pi<float>());
    transform->InsertEndChild(rotate);

    transform->InsertEndChild(this->createVector("scale", glm::vec3(rockyScale)));

    tinyxml2::XMLElement *models = group->InsertNewChildElement("models");
    tinyxml2::XMLElement *model = models->InsertNewChildElement("model");
    model->SetAttribute("file", "comet.3d");

    return group;
}

}
