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

#include "engine/Entity.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
namespace engine {

Entity::Entity(const std::string& modelFile, glm::vec3 initialPosition, float rotationX, float rotationY, float rotationZ, float scaling)
    : position(initialPosition), rotX(rotationX), rotY(rotationY), rotZ(rotationZ), scale(scaling) {
    
    std::ifstream file(modelFile);
    if (!file.is_open()) {
        std::cerr << "[ERRO] Falha ao abrir " << modelFile << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.push_back(glm::vec3(x, y, z));
        } else if (type == "f") {
            std::vector<int> face;
            int index;
            while (ss >> index) {
                face.push_back(index - 1);
            }
            indices.insert(indices.end(), face.begin(), face.end());
        }
    }

    file.close();
    setupMesh();
}

Entity::~Entity() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Entity::draw(GLuint shaderProgram) const {
    (void) shaderProgram; // Supressão do aviso de parâmetro não utilizado

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

std::vector<std::unique_ptr<Entity>> Entity::loadModels(const std::string& directory) {
    std::vector<std::unique_ptr<Entity>> models;

    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.path().extension() == ".3d") {
                std::cout << "[DEBUG] Modelo carregado: " << entry.path().filename() << std::endl;
                models.push_back(std::make_unique<Entity>(
                    entry.path().string(),
                    glm::vec3(0.0f),
                    0.0f, 0.0f, 0.0f,
                    1.0f
                ));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERRO] Falha ao listar modelos: " << e.what() << std::endl;
    }

    return models;
}

void Entity::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

} 