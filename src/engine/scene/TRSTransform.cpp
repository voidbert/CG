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

#include <glm/gtx/transform.hpp>
#include <stdexcept>

#include "engine/scene/Translation.hpp"
#include "engine/scene/TRSTransform.hpp"
#include "utils/XMLUtils.hpp"

namespace engine::scene {

class IdentityTransform : public ITransform {
    glm::mat4 getMatrix() const override {
        return glm::mat4(1.0f);
    }
};

TRSTransform::TRSTransform(const tinyxml2::XMLElement *transformElement) :
    transformations { std::make_unique<IdentityTransform>(),
                      std::make_unique<IdentityTransform>(),
                      std::make_unique<IdentityTransform>() } {

    bool hasTranslation, hasRotation, hasScale;
    int i;
    const tinyxml2::XMLElement *child = transformElement->FirstChildElement();

    for (i = 0; i < 3 && child; ++i, child = child->NextSiblingElement()) {
        std::string name = child->Name();
        if (name == "translate" && !hasTranslation) {
            this->transformations[i] = std::make_unique<Translation>(child);
            hasTranslation = true;
        } else if (name == "rotate" && !hasRotation) {

            hasRotation = true;
        } else if (name == "scale" && !hasScale) {

            hasScale = true;
        } else {
            throw std::runtime_error("Invalid / multiple occurences of <" + name +
                                     "> element in scene XML file");
        }
    }

    if (i == 3 && child) {
        throw std::runtime_error("More than 3 transformations in <transform>");
    }
}

glm::mat4 TRSTransform::getMatrix() const {
    return this->transformations[0]->getMatrix() * this->transformations[1]->getMatrix() *
        this->transformations[2]->getMatrix();
}

}
