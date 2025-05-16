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

#include "engine/render/Framebuffer.hpp"

namespace engine::render {

Framebuffer::Framebuffer(int _width, int _height) : width(_width), height(_height) {
    // Create framebuffer
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    // Create color attachment
    glGenTextures(1, &this->colorTexture);
    glBindTexture(GL_TEXTURE_2D, this->colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           this->colorTexture,
                           0);

    // Create depth buffer
    glGenRenderbuffers(1, &this->depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, this->depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              this->depthRenderBuffer);
}

Framebuffer::~Framebuffer() {
    glDeleteTextures(1, &this->colorTexture);
    glDeleteRenderbuffers(1, &this->depthRenderBuffer);
    glDeleteFramebuffers(1, &this->fbo);
}

void Framebuffer::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    glViewport(0, 0, this->width, this->height);
}

std::array<uint8_t, 3> Framebuffer::sample(int x, int y) {
    this->use();
    glFinish();

    std::array<uint8_t, 3> pixel;
    glReadPixels(x, this->height - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel.data());

    return pixel;
}

}
