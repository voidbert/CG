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

#include "generator/figures/Box.hpp"
#include "generator/figures/Sphere.hpp"
#include "generator/figures/Torus.hpp"
#include "utils/WavefrontOBJ.hpp"

namespace generator {
void printUsage(const std::string &programName) {
    std::cerr << "Wrong usage. Here's the correct one:" << std::endl;
    std::cerr << "  " << programName << " plane  <length> <divisions> <file>" << std::endl;
    std::cerr << "  " << programName << " box    <length> <grid>      <file>" << std::endl;
    std::cerr << "  " << programName << " sphere <radius> <slices>    <stacks> <file>" << std::endl;
    std::cerr << "  " << programName << " cone   <radius> <height>    <slices> <stacks> <file>"
              << std::endl;
    std::cerr << "  " << programName
              << " torus  <majorRadius> <minorRadius> <slices> <stacks> <file>" << std::endl;
}

double stringToDouble(const std::string &str) {
    size_t charactersParsed;
    double ret = std::stod(str, &charactersParsed);
    if (charactersParsed != str.length())
        throw std::invalid_argument("str is not a double");
    if (ret < 0)
        throw std::invalid_argument("str is negative");
    return ret;
}

int stringToInt(const std::string &str) {
    size_t charactersParsed;
    int ret = std::stoi(str, &charactersParsed);
    if (charactersParsed != str.length())
        throw std::invalid_argument("str is not an integer");
    if (ret < 0)
        throw std::invalid_argument("str is negative");
    return ret;
}

int main(int argc, char **argv) {
    std::vector<std::string> args(argv, argv + argc);

    try {
        if (args.at(1) == "plane") {
            double length = stringToDouble(args.at(2));
            int divisions = stringToInt(args.at(3));
            std::string file = args.at(4);
            // generatePlane(length,divisions,file)
        } else if (args.at(1) == "box") {
            double length = stringToDouble(args.at(2));
            double grid = stringToDouble(args.at(3));
            std::string file = args.at(4);

            figures::Box box(length, grid);
            box.writeToFile(file);
        } else if (args.at(1) == "sphere") {
            double radius = stringToDouble(args.at(2));
            int slices = stringToInt(args.at(3));
            int stacks = stringToInt(args.at(4));
            std::string file = args.at(5);

            figures::Sphere sphere(radius, slices, stacks);
            sphere.writeToFile(file);
        } else if (args.at(1) == "cone") {
            double radius = stringToDouble(args.at(2));
            double height = stringToDouble(args.at(3));
            int slices = stringToInt(args.at(4));
            int stacks = stringToInt(args.at(5));
            std::string file = args.at(6);
            // generateCone(radius,height,slices,stacks,file)
        } else if (args.at(1) == "torus") {
            double majorRadius = stringToDouble(args.at(2));
            double minorRadius = stringToDouble(args.at(3));
            int slices = stringToInt(args.at(4));
            int stacks = stringToInt(args.at(5));
            std::string file = args.at(6);

            figures::Torus torus(majorRadius, minorRadius, slices, stacks);
            torus.writeToFile(file);
        } else {
            printUsage(args[0]);
        }
    } catch (std::out_of_range &e) {
        printUsage(args[0]);
        return 1;
    } catch (std::invalid_argument &e) {
        printUsage(args[0]);
        return 1;
    }

    return 0;
}
}
