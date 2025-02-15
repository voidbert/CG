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

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace engine {

class Entity {
private:
    std::vector<glm::vec3> vertices;
    std::vector<int> indices;
    
    GLuint VAO, VBO, EBO;
    glm::vec3 position;
    float rotX, rotY, rotZ, scale;

    void setupMesh();

public:
    Entity(const std::string& modelFile, glm::vec3 position, float rotX, float rotY, float rotZ, float scale);
    virtual ~Entity();  

    void draw(GLuint shaderProgram) const;
    glm::vec3 getPosition() const;

    static std::vector<std::unique_ptr<Entity>> loadModels(const std::string& directory);
};

} // namespace engine
