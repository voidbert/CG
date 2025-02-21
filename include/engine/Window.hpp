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

#include <GLFW/glfw3.h>
#include <string>

namespace engine {
class Window {
private:
    GLFWwindow *handle;
    int width, height;

public:
    Window(const std::string &title, int _width, int _height);
    Window(const Window &window) = delete;
    Window(Window &&window) = delete;
    ~Window();

    void runLoop();
    void resize(int _width, int _height);

    int getWidth() const;
    int getHeight() const;

protected:
    GLFWwindow *getHandle();

    virtual void onUpdate(float time, float timeElapsed) = 0;
    virtual void onRender() = 0;
    virtual void onResize(int _width, int _height) = 0;
};
}
