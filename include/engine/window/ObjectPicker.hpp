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

#include <glad/glad.h>

namespace engine::window {

class ObjectPicker {
private:
    GLuint fbo;
    GLuint colorTexture;
    GLuint depthRenderbuffer;
    int width, height;

    void init();
    void cleanup();

public:
    ObjectPicker(int w, int h);
    ~ObjectPicker();

    void resize(int newWidth, int newHeight);
    void bind() const;
    void unbind() const;
    unsigned char readID(int x, int y) const;
};

}
