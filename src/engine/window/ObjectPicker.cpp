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

#include "engine/window/ObjectPicker.hpp"
#include <stdexcept>

namespace engine::window {

ObjectPicker::ObjectPicker(int w, int h) :
    fbo(0), colorTexture(0), depthRenderbuffer(0), width(w), height(h) {
    init();
}

ObjectPicker::~ObjectPicker() {
    cleanup();
}

void ObjectPicker::init() {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              depthRenderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("ObjectPicker: framebuffer incompleto");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ObjectPicker::cleanup() {
    if (depthRenderbuffer)
        glDeleteRenderbuffers(1, &depthRenderbuffer);
    if (colorTexture)
        glDeleteTextures(1, &colorTexture);
    if (fbo)
        glDeleteFramebuffers(1, &fbo);
}

void ObjectPicker::resize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    cleanup();
    init();
}

void ObjectPicker::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

void ObjectPicker::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned char ObjectPicker::readID(int x, int y) const {
    glFlush();
    glFinish();

    unsigned char pixel[4] = { 0, 0, 0, 0 };
    glReadPixels(x, height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    int id = pixel[0] + (pixel[1] << 8) + (pixel[2] << 16);
    return id;
}

}
