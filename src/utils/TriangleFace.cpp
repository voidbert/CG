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

#include "utils/TriangleFace.hpp"

namespace utils {

TriangleFace::TriangleFace(int32_t p1, int32_t p2, int32_t p3) :
    positions { p1, p2, p3 }, textureCoordinates { -1, -1, -1 }, normals { -1, -1, -1 } {}

TriangleFace::TriangleFace(int32_t p1,
                           int32_t t1,
                           int32_t n1,
                           int32_t p2,
                           int32_t t2,
                           int32_t n2,
                           int32_t p3,
                           int32_t t3,
                           int32_t n3) :
    positions { p1, p2, p3 }, textureCoordinates { t1, t2, t3 }, normals { n1, n2, n3 } {}

}
