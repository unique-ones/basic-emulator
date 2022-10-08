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
 * @brief Loads a texture from the given path and uploads it to the gpu
 * @param texture Texture handle
 * @param path Path to the image file
 * @return bool
 */
bool texture_create_from_file(texture_t* texture, const char* path);

/**
 * @brief Destroys the specified texture and its data
 * @param texture Texture handle
 */
void texture_destroy(texture_t* texture);

/**
 * @brief Binds a texture to the sampler at the specified slot
 * @param texture Texture handle
 * @param slot Sampler slot
 */
void texture_bind(texture_t* texture, u32 slot);

/**
 * @brief Unbinds the currently bound texture at the specified sampler slot
 * @param slot Sampler slot
 */
void texture_unbind(u32 slot);

#endif// RETRO_TEXTURE_H
