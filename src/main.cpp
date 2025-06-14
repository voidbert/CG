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

#include <filesystem>
#include <iostream>

#include "engine/main.hpp"
#include "generator/main.hpp"

int main(int argc, char **argv) {
    if (argc < 1) {
        std::cerr << "Not enough arguments (program name not provided)" << std::endl;
        return 1;
    }

    const std::string &programName = std::filesystem::path(argv[0]).filename();
    if (programName == "engine") {
        return engine::main(argc, argv);
    } else if (programName == "generator") {
        return generator::main(argc, argv);
    } else {
        std::cerr << "Unknown program name: " << programName << std::endl;
        return 1;
    }
}
