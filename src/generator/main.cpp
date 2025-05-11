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
#include <iomanip>
#include <iostream>
#include <numeric>

#include "generator/BezierPatch.hpp"
#include "generator/figures/Box.hpp"
#include "generator/figures/Cone.hpp"
#include "generator/figures/Cylinder.hpp"
#include "generator/figures/Gear.hpp"
#include "generator/figures/KleinBottle.hpp"
#include "generator/figures/MobiusStrip.hpp"
#include "generator/figures/Plane.hpp"
#include "generator/figures/Sphere.hpp"
#include "generator/figures/Torus.hpp"
#include "generator/SolarSystem.hpp"
#include "utils/WavefrontOBJ.hpp"

namespace generator {

const std::vector<std::vector<std::string>> FIGURE_USAGES = {
    { "plane", "<length>", "<divisions>" },
    { "box", "<length>", "<divisions>", "[multi-textured]" },
    { "sphere", "<radius>", "<slices>", "<stacks>" },
    { "cone", "<radius>", "<height>", "<slices>", "<stacks>" },
    { "cylinder", "<radius>", "<height>", "<slices>", "<stacks>" },
    { "torus", "<majorRadius>", "<minorRadius>", "<slices>", "<sides>" },
    { "mobiusStrip", "<radius>", "<width>", "<twists>", "<slices>", "<stacks>" },
    { "kleinBottle", "<radius>", "<slices>", "<stacks>" },
    { "gear", "<majorRadius>", "<minorRadius>", "<toothHeight>", "<height>", "<teeth>", "<stacks>" }
};

size_t getLengthOfUsageColumn(size_t column) {
    return std::transform_reduce(
        FIGURE_USAGES.cbegin(),
        FIGURE_USAGES.cend(),
        0,
        [](int l1, int l2) { return std::max(l1, l2); },
        [column](const std::vector<std::string> &usage) {
            return column < usage.size() ? usage[column].length() : 0;
        });
}

void printUsage(const std::string &programName) {
    std::cerr << "Wrong usage. Here's the correct one:" << std::endl << std::endl;

    std::cerr << "Figure generation:" << std::endl;
    std::cerr << std::left;
    for (const std::vector<std::string> &usage : FIGURE_USAGES) {
        std::cerr << "  " << programName << " ";

        for (size_t i = 0; i < usage.size(); ++i) {
            const size_t length = getLengthOfUsageColumn(i);
            std::cerr << std::setw(length) << usage[i] << " ";
        }

        std::cerr << std::setw(0) << "<file>" << std::endl;
    }

    std::cerr << std::endl << "Scene generation:" << std::endl;
    std::cerr << "  " << programName
              << " solarSystem [<sunScale> <rockyScale> <gasScale> <timeScale>] <directory>"
              << std::endl;

    std::cerr << std::endl << "Model conversion:" << std::endl;
    std::cerr << "  " << programName << " patch <patchFile> <tessellation> <file>" << std::endl;
}

float stringToFloat(const std::string &str) {
    size_t charactersParsed;
    float ret = std::stof(str, &charactersParsed);
    if (charactersParsed != str.length())
        throw std::invalid_argument("str is not a float");
    return ret;
}

int stringToInt(const std::string &str) {
    size_t charactersParsed;
    int ret = std::stoi(str, &charactersParsed);
    if (charactersParsed != str.length())
        throw std::invalid_argument("str is not an integer");
    if (ret <= 0)
        throw std::invalid_argument("str is not positive");
    return ret;
}

void validateArgumentCount(int providedArgs, int expectedArgs) {
    if (providedArgs != expectedArgs)
        throw std::invalid_argument("Wrong number of command-line arguments");
}

int main(int argc, char **argv) {
    const std::vector<std::string> args(argv, argv + argc);

    try {
        const std::string &file = args.at(args.size() - 1);

        if (args.at(1) == "plane") {
            validateArgumentCount(argc, 5);
            const float length = stringToFloat(args.at(2));
            const int divisions = stringToInt(args.at(3));

            figures::Plane plane(length, divisions);
            plane.writeToFile(file);
        } else if (args.at(1) == "box") {
            const float length = stringToFloat(args.at(2));
            const int grid = stringToInt(args.at(3));

            if (argc == 5) {
                figures::Box box(length, grid);
                box.writeToFile(file);
            } else if (argc == 6 && args.at(4) == "multi-textured") {
                figures::Box box(length, grid, true);
                box.writeToFile(file);
            } else {
                throw std::invalid_argument("Invalid command-line arguments");
            }
        } else if (args.at(1) == "sphere") {
            validateArgumentCount(argc, 6);

            const float radius = stringToFloat(args.at(2));
            const int slices = stringToInt(args.at(3));
            const int stacks = stringToInt(args.at(4));

            figures::Sphere sphere(radius, slices, stacks);
            sphere.writeToFile(file);
        } else if (args.at(1) == "cone") {
            validateArgumentCount(argc, 7);
            float radius = stringToFloat(args.at(2));
            float height = stringToFloat(args.at(3));
            int slices = stringToInt(args.at(4));
            int stacks = stringToInt(args.at(5));

            figures::Cone cone(radius, height, slices, stacks);
            cone.writeToFile(file);
        } else if (args.at(1) == "cylinder") {
            float radius = stringToFloat(args.at(2));
            float height = stringToFloat(args.at(3));
            int slices = stringToInt(args.at(4));
            int stacks = stringToInt(args.at(5));

            if (argc == 7) {
                figures::Cylinder cylinder(radius, height, slices, stacks);
                cylinder.writeToFile(file);
            } else if (argc == 8 && args.at(6) == "multi-textured") {
                figures::Cylinder cylinder(radius, height, slices, stacks, true);
                cylinder.writeToFile(file);
            } else {
                throw std::invalid_argument("Invalid command-line arguments");
            }

        } else if (args.at(1) == "torus") {
            validateArgumentCount(argc, 7);
            float majorRadius = stringToFloat(args.at(2));
            float minorRadius = stringToFloat(args.at(3));
            int slices = stringToInt(args.at(4));
            int stacks = stringToInt(args.at(5));

            figures::Torus torus(majorRadius, minorRadius, slices, stacks);
            torus.writeToFile(file);
        } else if (args.at(1) == "mobiusStrip") {
            validateArgumentCount(argc, 8);

            const float radius = stringToFloat(args.at(2));
            const float width = stringToFloat(args.at(3));
            const int twist = stringToInt(args.at(4));
            const int slices = stringToInt(args.at(5));
            const int stacks = stringToInt(args.at(6));

            figures::MobiusStrip mobius(radius, width, twist, slices, stacks);
            mobius.writeToFile(file);
        } else if (args.at(1) == "kleinBottle") {
            validateArgumentCount(argc, 6);
            float radius = stringToFloat(args.at(2));
            int slices = stringToInt(args.at(3));
            int stacks = stringToInt(args.at(4));

            figures::KleinBottle kleinbottle(radius, slices, stacks);
            kleinbottle.writeToFile(file);
        } else if (args.at(1) == "gear") {
            validateArgumentCount(argc, 9);

            const float majorRadius = stringToFloat(args.at(2));
            const float minorRadius = stringToFloat(args.at(3));
            const float toothHeight = stringToFloat(args.at(4));
            const float height = stringToFloat(args.at(5));
            const int teeth = stringToInt(args.at(6));
            const int stacks = stringToInt(args.at(7));

            figures::Gear gear(majorRadius, minorRadius, toothHeight, height, teeth, stacks);
            gear.writeToFile(file);
        } else if (args.at(1) == "solarSystem") {
            if (argc == 3) {
                SolarSystem solarSystem;
                solarSystem.writeToFile(file);
            } else if (argc == 7) {
                const float sunScale = stringToFloat(args.at(2));
                const float rockyScale = stringToFloat(args.at(3));
                const float gasScale = stringToFloat(args.at(4));
                const float timeScale = stringToFloat(args.at(5));

                SolarSystem solarSystem(sunScale, rockyScale, gasScale, timeScale);
                solarSystem.writeToFile(file);
            } else {
                throw std::invalid_argument("Wrong number of command-line arguments");
            }
        } else if (args.at(1) == "patch") {
            validateArgumentCount(argc, 5);
            const std::string &patchFile = args.at(2);
            const int tessellation = stringToInt(args.at(3));

            BezierPatch patch(patchFile, tessellation);
            patch.writeToFile(file);
        } else {
            printUsage(args[0]);
            return 1;
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
