#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool texture_create_from_file(texture_t* texture, const char* path) {
    texture->handle = 0;
    texture->width = 0;
    texture->height = 0;
    texture->channels = 4;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture->handle);
    glBindTextureUnit(0, texture->handle);

    stbi_set_flip_vertically_on_load(0);
    texture->data = stbi_load(path, &texture->width, &texture->height, &texture->channels, 4);
    if (texture->data) {
        glTextureStorage2D(texture->handle, 1, GL_RGBA8, texture->width, texture->height);
        glTextureParameteri(texture->handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(texture->handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(texture->handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(texture->handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureSubImage2D(texture->handle, 0, 0, 0, texture->width, texture->height, GL_RGBA, GL_UNSIGNED_BYTE,
                            texture->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        return true;
    }
    return false;
}

void texture_destroy(texture_t* texture) {
    free(texture->data);
    glDeleteTextures(1, &texture->handle);
}

void texture_bind(texture_t* texture, u32 slot) {
    glBindTextureUnit(slot, texture->handle);
}

void texture_unbind(u32 slot) {
    glBindTextureUnit(slot, 0);
}