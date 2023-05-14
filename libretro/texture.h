#ifndef RETRO_TEXTURE_H
#define RETRO_TEXTURE_H

#include "types.h"

typedef struct texture {
    u32 handle;
    s32 width;
    s32 height;
    s32 channels;
    u8* data;
} texture_t;

/**
 * @brief loads a texture from the given path and uploads it to the gpu
 * @param self texture handle
 * @param path path to the image file
 * @return bool
 */
bool texture_create(texture_t* self, const char* path);

/**
 * @brief destroys the specified texture and its data
 * @param self texture handle
 */
void texture_destroy(texture_t* self);

/**
 * @brief binds a texture to the sampler at the specified slot
 * @param self texture handle
 * @param slot sampler slot
 */
void texture_bind(texture_t* self, u32 slot);

/**
 * @brief unbinds the currently bound texture at the specified sampler slot
 * @param slot sampler slot
 */
void texture_unbind(u32 slot);

#endif// RETRO_TEXTURE_H
