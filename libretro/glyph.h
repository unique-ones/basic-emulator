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
 * @brief Creates a glyph cache for the specified font
 * @param cache Cache handle
 * @param path Path to the .ttf font file
 * @return bool
 */
bool glyph_cache_create(glyph_cache_t* cache, const char* path);

/**
 * @brief Fetches the specified symbol from the glyph cache
 * @param cache Cache handle
 * @param info Glyph info handle
 * @param symbol Symbol that shall be fetched
 */
void glyph_cache_acquire(glyph_cache_t* cache, glyph_info_t* info, char symbol);

#endif// RETRO_GLYPH_H
