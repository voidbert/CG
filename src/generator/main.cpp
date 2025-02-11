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

#include <iostream>

#include "generator/figures/sphere.hpp"

namespace generator {

int main(int argc, char **argv) {
    std::cout << "I am the generator!" << std::endl;

    // remover depois do merge da Ana
    float radius = std::stof(argv[1]);
    int slices = std::stoi(argv[2]);
    int stacks = std::stoi(argv[3]);
    const char *filepath = argv[4];

    Sphere sphere(radius, slices, stacks);
    sphere.toObj(filepath);

    std::cout << "Esfera gerada com sucesso e salva em " << filepath << "\n";

    return 1;
}
}
