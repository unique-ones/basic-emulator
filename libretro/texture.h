//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
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
