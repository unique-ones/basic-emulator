//
// MIT License
//
// Copyright (c) 2024 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/// Loads a texture from the given path and uploads it to the gpu
bool texture_create(texture_t *self, const char *path) {
    self->handle = 0;
    self->width = 0;
    self->height = 0;
    self->channels = 4;
    glCreateTextures(GL_TEXTURE_2D, 1, &self->handle);
    glBindTextureUnit(0, self->handle);

    stbi_set_flip_vertically_on_load(0);
    self->data = stbi_load(path, &self->width, &self->height, &self->channels, 4);
    if (self->data) {
        glTextureStorage2D(self->handle, 1, GL_RGBA8, self->width, self->height);
        glTextureParameteri(self->handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(self->handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(self->handle, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTextureParameteri(self->handle, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTextureSubImage2D(self->handle, 0, 0, 0, self->width, self->height, GL_RGBA, GL_UNSIGNED_BYTE, self->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        return true;
    }
    return false;
}

/// Destroys the specified texture and its data
void texture_destroy(texture_t *self) {
    free(self->data);
    glDeleteTextures(1, &self->handle);
}

/// Binds the texture to the sampler at the specified slot
void texture_bind(texture_t *self, u32 slot) {
    glBindTextureUnit(slot, self->handle);
}

/// Unbinds the currently bound texture at the specified sampler slot
void texture_unbind(u32 slot) {
    glBindTextureUnit(slot, 0);
}
