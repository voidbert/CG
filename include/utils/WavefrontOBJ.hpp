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

#ifndef ENGINE_MAIN_HPP
#define ENGINE_MAIN_HPP

#include <glm/glm.hpp>
#include <vector>

void readObjFile(const std::string &filename,
                 std::vector<glm::vec4> &vertices,
                 std::vector<std::vector<int>> &faces);
void writeObjFile(const std::string &filename,
                  const std::vector<glm::vec4> &vertices,
                  const std::vector<std::vector<int>> &faces);

#endif
