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
#include <iostream>
#include <sstream>

#include "utils/utils.hpp"

namespace generator {

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Not enough arguments\n\n";
        std::cerr << "The program generator supports the following commands:\n";
        std::cerr << "  generator plane <length> <divisions> <file3d>\n";
        std::cerr << "  generator box <length> <grid> <file3d>\n";
        std::cerr << "  generator sphere <radius> <slices> <stacks> <file3d>\n";
        std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";

        return 1;
    }
    std::string graphical_primitive = argv[1];

    if (graphical_primitive == "plane") {
        if (argc == 5) {
            std::stringstream ss1(argv[2]);
            float length;
            ss1 >> length;
            if (ss1.fail() || ss1.bad() || !ss1.eof()) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator plane <length> <divisions> <file3d>\n";
                std::cerr << "<length> should be a valid number\n";
                return 1;
            } else if (std::isinf(length)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator plane <length> <divisions> <file3d>\n";
                std::cerr << "<length> should be a valid number (not infinity)\n";
                return 1;
            } else if (std::isnan(length)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator plane <length> <divisions> <file3d>\n";
                std::cerr << "<length> should be a valid number (not NaN)\n";
                return 1;
            }
            std::stringstream ss2(argv[3]);
            int divisions;
            ss2 >> divisions;
            if (ss2.fail() || ss2.bad() || !ss2.eof()) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator plane <length> <divisions> <file3d>\n";
                std::cerr << "<divisions> should be a valid integer\n";
                return 1;
            }
            std::string file3d = argv[4];
            if (!(file3d.size() >= 3 && file3d.compare(file3d.size() - 3, 3, ".3d") == 0)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator plane <length> <divisions> <file3d>\n";
                std::cerr << "<file3d> should end with the '.3d' extension\n";
                return 1;
            }
            // generatePlane(length,divisions,file3d)
        } else {
            std::cerr << "Wrong number of arguments\n\n";
            std::cerr << "Try this:\n";
            std::cerr << "  generator plane <length> <divisions> <file3d>\n";
            return 1;
        }
    } else if (graphical_primitive == "box") {
        if (argc == 5) {
            std::stringstream ss1(argv[2]);
            float length;
            ss1 >> length;
            if (ss1.fail() || ss1.bad() || !ss1.eof()) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator box <length> <grid> <file3d>\n";
                std::cerr << "<length> should be a valid number\n";
                return 1;
            } else if (std::isinf(length)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator box <length> <grid> <file3d>\n";
                std::cerr << "<length> should be a valid number (not infinity)\n";
                return 1;
            } else if (std::isnan(length)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator box <length> <grid> <file3d>\n";
                std::cerr << "<length> should be a valid number (not NaN)\n";
                return 1;
            }
            std::stringstream ss2(argv[3]);
            float grid;
            ss2 >> grid;
            if (ss2.fail() || ss2.bad() || !ss2.eof()) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator box <length> <grid> <file3d>\n";
                std::cerr << "<grid> should be a valid number\n";
                return 1;
            } else if (std::isinf(grid)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator box <length> <grid> <file3d>\n";
                std::cerr << "<grid> should be a valid number (not infinity)\n";
                return 1;
            } else if (std::isnan(grid)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator box <length> <grid> <file3d>\n";
                std::cerr << "<grid> should be a valid number (not NaN)\n";
                return 1;
            }
            std::string file3d = argv[4];
            if (!(file3d.size() >= 3 && file3d.compare(file3d.size() - 3, 3, ".3d") == 0)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator box <length> <grid> <file3d>\n";
                std::cerr << "<file3d> should end with the '.3d' extension\n";
                return 1;
            }
            // generateBox(length,grid,file3d)
        } else {
            std::cerr << "Wrong number of arguments\n\n";
            std::cerr << "Try this:\n";
            std::cerr << "  generator box <length> <grid> <file3d>\n";
            return 1;
        }
    } else if (graphical_primitive == "sphere") {
        if (argc == 6) {
            std::stringstream ss1(argv[2]);
            float radius;
            ss1 >> radius;
            if (ss1.fail() || ss1.bad() || !ss1.eof()) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator sphere <radius> <slices> <stacks> <file3d>\n";
                std::cerr << "<radius> should be a valid number\n";
                return 1;
            } else if (std::isinf(radius)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator sphere <radius> <slices> <stacks> <file3d>\n";
                std::cerr << "<radius> should be a valid number (not infinity)\n";
                return 1;
            } else if (std::isnan(radius)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator sphere <radius> <slices> <stacks> <file3d>\n";
                std::cerr << "<radius> should be a valid number (not NaN)\n";
                return 1;
            }
            std::stringstream ss2(argv[3]);
            int slices;
            ss2 >> slices;
            if (ss2.fail() || ss2.bad() || !ss2.eof()) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator sphere <radius> <slices> <stacks> <file3d>\n";
                std::cerr << "<slices> should be a valid integer\n";
                return 1;
            }
            std::stringstream ss3(argv[4]);
            int stacks;
            ss3 >> stacks;
            if (ss3.fail() || ss3.bad() || !ss3.eof()) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator sphere <radius> <slices> <stacks> <file3d>\n";
                std::cerr << "<stacks> should be a valid integer\n";
                return 1;
            }
            std::string file3d = argv[5];
            if (!(file3d.size() >= 3 && file3d.compare(file3d.size() - 3, 3, ".3d") == 0)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator sphere <radius> <slices> <stacks> <file3d>\n";
                std::cerr << "<file3d> should end with the '.3d' extension\n";
                return 1;
            }
            // generateSphere(radius,slices,stacks,file3d)
        } else {
            std::cerr << "Wrong number of arguments\n\n";
            std::cerr << "Try this:\n";
            std::cerr << "  generator sphere <radius> <slices> <stacks> <file3d>\n";
            return 1;
        }
    } else if (graphical_primitive == "cone") {
        if (argc == 7) {
            std::stringstream ss1(argv[2]);
            float radius;
            ss1 >> radius;
            if (ss1.fail() || ss1.bad() || !ss1.eof()) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";
                std::cerr << "<radius> should be a valid number\n";
                return 1;
            } else if (std::isinf(radius)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";
                std::cerr << "<radius> should be a valid number (not infinity)\n";
                return 1;
            } else if (std::isnan(radius)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";
                std::cerr << "<radius> should be a valid number (not NaN)\n";
                return 1;
            }
            std::stringstream ss2(argv[3]);
            float height;
            ss2 >> height;
            if (ss2.fail() || ss2.bad() || !ss2.eof()) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";
                std::cerr << "<height> should be a valid number\n";
                return 1;
            } else if (std::isinf(radius)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";
                std::cerr << "<height> should be a valid number (not infinity)\n";
                return 1;
            } else if (std::isnan(radius)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";
                std::cerr << "<height> should be a valid number (not NaN)\n";
                return 1;
            }
            std::stringstream ss3(argv[4]);
            int slices;
            ss3 >> slices;
            if (ss3.fail() || ss3.bad() || !ss3.eof()) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";
                std::cerr << "<slices> should be a valid integer\n";
                return 1;
            }
            std::stringstream ss4(argv[5]);
            int stacks;
            ss4 >> stacks;
            if (ss4.fail() || ss4.bad() || !ss4.eof()) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";
                std::cerr << "<stacks> should be a valid integer\n";
                return 1;
            }
            std::string file3d = argv[6];
            if (!(file3d.size() >= 3 && file3d.compare(file3d.size() - 3, 3, ".3d") == 0)) {
                std::cerr << "Invalid command\n\n";
                std::cerr << "In the command:\n";
                std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";
                std::cerr << "<file3d> should end with the '.3d' extension\n";
                return 1;
            }
            // generateCone(radius,height,slices,stacks,file3d)
        } else {
            std::cerr << "Wrong number of arguments\n\n";
            std::cerr << "Try this:\n";
            std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";
            return 1;
        }
    } else {
        std::cerr << "Graphical primitive '" << graphical_primitive
                  << "' has not been implemented\n\n";
        std::cerr << "The program generator supports the following commands:\n";
        std::cerr << "  generator plane <length> <divisions> <file3d>\n";
        std::cerr << "  generator box <length> <grid> <file3d>\n";
        std::cerr << "  generator sphere <radius> <slices> <stacks> <file3d>\n";
        std::cerr << "  generator cone <radius> <height> <slices> <stacks> <file3d>\n";
        return 1;
    }
    return 0;
}

}
