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

#include <GLFW/glfw3.h>
#include <stdexcept>

#include "engine/scene/transform/AnimatedRotation.hpp"
#include "engine/scene/transform/AnimatedTranslation.hpp"
#include "engine/scene/transform/Rotation.hpp"
#include "engine/scene/transform/Scale.hpp"
#include "engine/scene/transform/Translation.hpp"
#include "engine/scene/transform/TRSTransform.hpp"

namespace engine::scene::transform {

TRSTransform::TRSTransform() :
    transforms { std::make_unique<BaseTransform>(),
                 std::make_unique<BaseTransform>(),
                 std::make_unique<BaseTransform>() } {}

TRSTransform::TRSTransform(const tinyxml2::XMLElement *transformElement) : TRSTransform() {
    bool hasTranslation = false, hasRotation = false, hasScale = false;
    int i;
    const tinyxml2::XMLElement *child = transformElement->FirstChildElement();

    for (i = 0; i < 3 && child; ++i, child = child->NextSiblingElement()) {
        const std::string name = child->Name();

        if (name == "translate" && !hasTranslation) {
            const char *timeAttr = child->Attribute("time");
            if (timeAttr != nullptr) {
                this->transforms[i] = std::make_unique<AnimatedTranslation>(child);
            } else {
                this->transforms[i] = std::make_unique<Translation>(child);
            }

            hasTranslation = true;
        } else if (name == "rotate" && !hasRotation) {
            const char *timeAttr = child->Attribute("time");
            if (timeAttr != nullptr) {
                this->transforms[i] = std::make_unique<AnimatedRotation>(child);
            } else {
                this->transforms[i] = std::make_unique<Rotation>(child);
            }

            hasRotation = true;
        } else if (name == "scale" && !hasScale) {
            this->transforms[i] = std::make_unique<Scale>(child);
            hasScale = true;
        } else {
            throw std::runtime_error("Invalid / multiple occurences of <" + name +
                                     "> element in scene XML file");
        }
    }

    if (i == 3 && child) {
        throw std::runtime_error("More than 3 transformations in <transform>");
    }

    this->update(glfwGetTime());
}

void TRSTransform::update(float time) {
    for (const std::unique_ptr<BaseTransform> &transform : this->transforms) {
        transform->update(time);
    }

    this->matrix = this->transforms[0]->getMatrix() * this->transforms[1]->getMatrix() *
        this->transforms[2]->getMatrix();
}

void TRSTransform::draw(render::RenderPipelineManager &pipelineManager,
                        const glm::mat4 &transformMatrix) const {

    for (const std::unique_ptr<BaseTransform> &transform : this->transforms) {
        transform->draw(pipelineManager, transformMatrix);
    }
}

}
