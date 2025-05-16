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

#include <array>
#include <cstdint>
#include <glad/glad.h>

namespace engine::render {

class Framebuffer {
private:
    GLuint fbo, colorTexture, depthRenderBuffer;
    int width, height;

public:
    Framebuffer(int _width, int _height);
    Framebuffer(const Framebuffer &framebuffer) = delete;
    Framebuffer(Framebuffer &&framebuffer) = delete;
    ~Framebuffer();

    void use();
    std::array<uint8_t, 3> sample(int x, int y);
};

}
