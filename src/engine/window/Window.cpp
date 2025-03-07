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

#include <glad/glad.h>
#include <stdexcept>

#include "engine/window/Window.hpp"

namespace engine {

Window::Window(const std::string &title, int argWidth, int argHeight) :
    width(argWidth), height(argHeight) {

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    this->handle = glfwCreateWindow(argWidth, argHeight, title.c_str(), NULL, NULL);
    if (!this->handle) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW's window");
    }

    glfwMakeContextCurrent(this->handle);

    int version = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    if (version == 0) {
        glfwTerminate();
        throw std::runtime_error("Failed to load OpenGL");
    }

    glfwSetWindowUserPointer(this->handle, this);
    glfwSetInputMode(this->handle, GLFW_STICKY_KEYS, GLFW_TRUE);
}

Window::~Window() {
    glfwDestroyWindow(this->handle);
    glfwTerminate();
}

void Window::runLoop() {
    glfwShowWindow(this->handle);
    this->onResize(this->width, this->height);

    double time = glfwGetTime();
    while (!glfwWindowShouldClose(this->handle)) {
        glfwSetWindowSizeCallback(this->handle, [](GLFWwindow *_handle, int _width, int _height) {
            Window *window = reinterpret_cast<Window *>(glfwGetWindowUserPointer(_handle));
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

void Window::resize(int _width, int _height) {
    glfwSetWindowSize(this->handle, _width, _height);
}

int Window::getWidth() const {
    return this->width;
}

int Window::getHeight() const {
    return this->height;
}

GLFWwindow *Window::getHandle() {
    return this->handle;
}

}
