// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_GPU_TEXTURE_H
#define RETRO_GPU_TEXTURE_H

#include "core/types.h"

typedef struct Texture {
    u32 handle;
    s32 width;
    s32 height;
    s32 channels;
    u8 *data;
} Texture;

/// Loads a texture from the given path and uploads it to the gpu
/// @param self The texture handle
/// @param path The path to the image file
/// @return A boolean value that indicates whether the texture could be created
bool texture_create(Texture *self, const char *path);

/// Destroys the specified texture and its data
/// @param self The texture handle
void texture_destroy(Texture const *self);

/// Binds the texture to the sampler at the specified slot
/// @param self The texture handle
/// @param slot The sampler slot
void texture_bind(Texture const *self, u32 slot);

/// Unbinds the currently bound texture at the specified sampler slot
/// @param slot The sampler slot
void texture_unbind(u32 slot);

#endif// RETRO_GPU_TEXTURE_H
