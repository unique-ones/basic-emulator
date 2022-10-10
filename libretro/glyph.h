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
 * @param self cache handle
 * @param path path to the .ttf font file
 * @return bool
 */
bool glyph_cache_create(glyph_cache_t* self, const char* path);

/**
 * @brief destroys the glyph self and its glyph atlas
 * @param self cache handle
 */
void glyph_cache_destroy(glyph_cache_t* self);

/**
 * @brief fetches the specified symbol from the glyph self
 * @param self cache handle
 * @param info glyph info handle
 * @param symbol symbol that shall be fetched
 */
void glyph_cache_acquire(glyph_cache_t* self, glyph_info_t* info, char symbol);

#endif// RETRO_GLYPH_H
