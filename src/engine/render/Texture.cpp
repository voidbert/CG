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

#include "engine/render/Texture.hpp"
#include <cstdint>
#include <stb/stb_image.h>
#include <stdexcept>

namespace engine::render {

Texture::Texture(const std::string &path) {
    // stb_image parameter initilization
    stbi_set_flip_vertically_on_load(true);
    stbi_convert_iphone_png_to_rgb(true);
    stbi_set_unpremultiply_on_load(true);

    // Image loading
    int width, height, components;
    uint8_t *const imageData = stbi_load(path.c_str(), &width, &height, &components, 3);
    if (!imageData) {
        const std::string reason = stbi_failure_reason();
        throw std::runtime_error("Failed to open image " + path + ": " + reason);
    }

    // Texture creation (with trilinear filtering)
    glGenTextures(1, &this->tid);
    glBindTexture(GL_TEXTURE_2D, this->tid);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Clenaup
    stbi_image_free(imageData);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->tid);
}

void Texture::use() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->tid);
}

}
