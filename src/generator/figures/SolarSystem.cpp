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
    tinyxml2::XMLElement *world = doc.NewElement("world");
    doc.InsertFirstChild(world);

    tinyxml2::XMLElement *window = doc.NewElement("window");
    window->SetAttribute("width", 512);
    window->SetAttribute("height", 512);
    world->InsertEndChild(window);

    tinyxml2::XMLElement *camera = doc.NewElement("camera");
    camera->SetAttribute("type", "orbital");
    world->InsertEndChild(camera);

    tinyxml2::XMLElement *position = doc.NewElement("position");
    position->SetAttribute("x", 20);
    position->SetAttribute("y", 20);
    position->SetAttribute("z", 20);
    camera->InsertEndChild(position);

    tinyxml2::XMLElement *lookAt = doc.NewElement("lookAt");
    lookAt->SetAttribute("x", 0);
    lookAt->SetAttribute("y", 0);
    lookAt->SetAttribute("z", 0);
    camera->InsertEndChild(lookAt);

    tinyxml2::XMLElement *up = doc.NewElement("up");
    up->SetAttribute("x", 0);
    up->SetAttribute("y", 1);
    up->SetAttribute("z", 0);
    camera->InsertEndChild(up);

    tinyxml2::XMLElement *projection = doc.NewElement("projection");
    projection->SetAttribute("fov", 60);
    projection->SetAttribute("near", 1);
    projection->SetAttribute("far", 1000);
    camera->InsertEndChild(projection);

    tinyxml2::XMLElement *solarSystem = doc.NewElement("group");
    world->InsertEndChild(solarSystem);

    addPlanet(solarSystem, 0, 0, 0, 5 * scale);

    addPlanet(solarSystem, 8 * scale, 0, 0, 0.5 * scale);  // Mercúrio
    addPlanet(solarSystem, 12 * scale, 0, 0, 0.9 * scale); // Vénus
    
    tinyxml2::XMLElement *earth = addPlanet(solarSystem, 16 * scale, 0, 0, 1 * scale);
    addPlanet(earth, 1.5 * scale, 0, 0, 0.3 * scale); // Lua

    addPlanet(solarSystem, 20 * scale, 0, 0, 0.8 * scale);  // Marte
    addPlanet(solarSystem, 30 * scale, 0, 0, 3 * scale);    // Júpiter
    
    // Criar Saturno e adicionar anéis
    tinyxml2::XMLElement *saturn = addPlanet(solarSystem, 40 * scale, 0, 0, 2.7 * scale);
    addSaturnRings(saturn); 

    addPlanet(solarSystem, 50 * scale, 0, 0, 2 * scale);   // Úrano
    addPlanet(solarSystem, 60 * scale, 0, 0, 1.9 * scale); // Neptuno

    writeToFile("scene_solarSystem.xml");
}

void SolarSystem::writeToFile(const std::string &filename) {
    std::string filePath = "res/scenes/" + filename;
    doc.SaveFile(filePath.c_str());
}

tinyxml2::XMLElement* SolarSystem::addPlanet(tinyxml2::XMLElement *parent,
                                             float x, float y, float z, float size) {
    tinyxml2::XMLElement *group = doc.NewElement("group");
    parent->InsertEndChild(group);

    tinyxml2::XMLElement *transform = doc.NewElement("transform");
    group->InsertEndChild(transform);

    tinyxml2::XMLElement *translate = doc.NewElement("translate");
    translate->SetAttribute("x", x);
    translate->SetAttribute("y", y);
    translate->SetAttribute("z", z);
    transform->InsertEndChild(translate);

    tinyxml2::XMLElement *scale = doc.NewElement("scale");
    scale->SetAttribute("x", size);
    scale->SetAttribute("y", size);
    scale->SetAttribute("z", size);
    transform->InsertEndChild(scale);

    tinyxml2::XMLElement *models = doc.NewElement("models");
    group->InsertEndChild(models);

    tinyxml2::XMLElement *model = doc.NewElement("model");
    model->SetAttribute("file", "../models/sphere.3d");
    models->InsertEndChild(model);

    return group;
}

void SolarSystem::addSaturnRings(tinyxml2::XMLElement *parent) {
    tinyxml2::XMLElement *group = doc.NewElement("group");
    parent->InsertEndChild(group);

    tinyxml2::XMLElement *transform = doc.NewElement("transform");
    group->InsertEndChild(transform);

    tinyxml2::XMLElement *scale = doc.NewElement("scale");
    scale->SetAttribute("x", 0.15); 
    scale->SetAttribute("y", 0.1); 
    scale->SetAttribute("z", 0.15);
    transform->InsertEndChild(scale);

    tinyxml2::XMLElement *models = doc.NewElement("models");
    group->InsertEndChild(models);

    tinyxml2::XMLElement *model = doc.NewElement("model");
    model->SetAttribute("file", "../models/torus.3d");
    models->InsertEndChild(model);
}


}
