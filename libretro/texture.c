#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool texture_create(texture_t* self, const char* path) {
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
        glTextureParameteri(self->handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(self->handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureSubImage2D(self->handle, 0, 0, 0, self->width, self->height, GL_RGBA, GL_UNSIGNED_BYTE, self->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        return true;
    }
    return false;
}

void texture_destroy(texture_t* self) {
    free(self->data);
    glDeleteTextures(1, &self->handle);
}

void texture_bind(texture_t* self, u32 slot) {
    glBindTextureUnit(slot, self->handle);
}

void texture_unbind(u32 slot) {
    glBindTextureUnit(slot, 0);
}
