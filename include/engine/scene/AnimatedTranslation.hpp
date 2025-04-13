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

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <tinyxml2.h>
#include <vector>

#include "engine/scene/ITransform.hpp"

namespace engine::scene {

class AnimatedTranslation : public ITransform {
private:
    float time;
    bool align;
    std::vector<glm::vec3> points;
    mutable glm::vec3 lastUp = glm::vec3(0, 1, 0);
    void getCatmullRomPoint(float t,
                            const glm::vec3 &p0,
                            const glm::vec3 &p1,
                            const glm::vec3 &p2,
                            const glm::vec3 &p3,
                            glm::vec3 &pos,
                            glm::vec3 &deriv) const;
    void getGlobalCatmullRomPoint(float globalT,
                                  const std::vector<glm::vec3> &points,
                                  glm::vec3 &pos,
                                  glm::vec3 &deriv) const;

public:
    explicit AnimatedTranslation(const tinyxml2::XMLElement *translateElement);
    glm::mat4 getMatrix() const override;
    std::vector<glm::vec3> getLine() const;
};

}
