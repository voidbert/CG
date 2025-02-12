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

#include <stdexcept>

#include "engine/Window.hpp"

namespace engine {
Window::Window(const std::string &title, int argWidth, int argHeight) :
    width(argWidth), height(argHeight) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    this->handle = glfwCreateWindow(argWidth, argHeight, title.c_str(), NULL, NULL);
    if (!this->handle) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW's window");
    }

    glfwSetWindowUserPointer(this->handle, this);
}

Window::~Window() {
    glfwDestroyWindow(this->handle);
    glfwTerminate();
}

void Window::runLoop() {
    glfwMakeContextCurrent(this->handle);

    double time = glfwGetTime();
    this->onResize(this->width, this->height);
    while (!glfwWindowShouldClose(this->handle)) {
        glfwSetWindowSizeCallback(this->handle, [](GLFWwindow *_handle, int _width, int _height) {
            Window *window = (Window *) glfwGetWindowUserPointer(_handle);
            window->width = _width;
            window->height = _height;
            window->onResize(_width, _height);
        });

        glfwPollEvents();
        double newTime = glfwGetTime();
        this->onUpdate(newTime, newTime - time);
        time = newTime;

        this->onRender();
        glfwSwapBuffers(this->handle);
    }
}

int Window::getWidth() {
    return this->width;
}

int Window::getHeight() {
    return this->height;
}

}
