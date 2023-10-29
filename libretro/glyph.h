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

#ifndef RETRO_GLYPH_H
#define RETRO_GLYPH_H

#include "math.h"
#include "texture.h"

#define FONT_SIZE 48.0f

typedef struct glyph_info {
    f32vec2_t size;
    f32vec2_t bearing;
    f32vec2_t advance;
    f32vec2_t texture_span;
    f32 texture_offset;
} glyph_info_t;

typedef struct glyph_cache {
    texture_t atlas;
    glyph_info_t info[128];
} glyph_cache_t;

/**
 * @brief creates a glyph self for the specified font
 * @param path path to the .ttf font file
 * @return bool
 */
glyph_cache_t* glyph_cache_new(const char* path);

/**
 * @brief destroys the glyph self and its glyph atlas
 * @param self cache handle
 */
void glyph_cache_free(glyph_cache_t* self);

/**
 * @brief fetches the specified symbol from the glyph self
 * @param self cache handle
 * @param info glyph info handle
 * @param symbol symbol that shall be fetched
 */
void glyph_cache_acquire(glyph_cache_t* self, glyph_info_t* info, char symbol);

#endif// RETRO_GLYPH_H
