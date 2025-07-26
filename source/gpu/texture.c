// Copyright (c) 2025 Elias Engelbert Plank

#include <glad/glad.h>

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/// Loads a texture from the given path and uploads it to the gpu
bool texture_create(Texture *self, const char *path) {
    self->handle = 0;
    self->width = 0;
    self->height = 0;
    self->channels = 4;

    glGenTextures(1, &self->handle);
    texture_bind(self, 0);

    stbi_set_flip_vertically_on_load(0);
    self->data = stbi_load(path, &self->width, &self->height, &self->channels, 4);
    if (self->data) {
        // Allocate storage and upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, self->width, self->height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, self->data);

        // Set texture parameters (must be done while bound in 4.1)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
        return true;
    }
    return false;
}

/// Destroys the specified texture and its data
void texture_destroy(Texture const *self) {
    free(self->data);
    glDeleteTextures(1, &self->handle);
}

/// Binds the texture to the sampler at the specified slot
void texture_bind(Texture const *self, u32 const slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, self->handle);
}

/// Unbinds the currently bound texture at the specified sampler slot
void texture_unbind(u32 const slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}
