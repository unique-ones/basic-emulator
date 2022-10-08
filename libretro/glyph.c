#include "glyph.h"
#include "utility.h"

#include <ft2build.h>
#include FT_FREETYPE_H

bool glyph_cache_create(glyph_cache_t* cache, const char* path) {
    binary_buffer_t font_data;
    if (!file_read(&font_data, path)) {
        return false;
    }

    FT_Library library;
    if (FT_Init_FreeType(&library)) {
        free(font_data.data);
        font_data.size = 0;
        return false;
    }
    FT_Face face;
    if (FT_New_Memory_Face(library, (FT_Byte*) font_data.data, (FT_Long) font_data.size, 0, &face)) {
        free(font_data.data);
        font_data.size = 0;
        FT_Done_FreeType(library);
        return false;
    }
    FT_Set_Pixel_Sizes(face, 0, (s32) FONT_SIZE);

    // Calculate combined size of glyphs
    s32vec2_t size = { 0, 0 };
    for (s32 i = 32; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "could not load character: %c\n", (char) i);
            continue;
        }
        glyph_info_t* info = (cache->info + i - 32);
        info->size.x = (f32) (face->glyph->bitmap.width);
        info->size.y = (f32) (face->glyph->bitmap.rows);
        info->bearing.x = (f32) (face->glyph->bitmap_left);
        info->bearing.y = (f32) (face->glyph->bitmap_top);
        info->advance.x = (f32) (face->glyph->advance.x >> 6);
        info->advance.y = (f32) (face->glyph->advance.y >> 6);
        info->texture_span.x = 0.0f;
        info->texture_span.y = 0.0f;
        info->texture_offset = 0.0f;
        size.x += (s32) face->glyph->bitmap.width;
        size.y = max_s32(size.y, (s32) face->glyph->bitmap.rows);
    }

    cache->atlas.data = NULL;
    cache->atlas.handle = 0;
    cache->atlas.width = size.x;
    cache->atlas.height = size.y;
    cache->atlas.channels = 1;
    glCreateTextures(GL_TEXTURE_2D, 1, &cache->atlas.handle);
    glBindTextureUnit(0, cache->atlas.handle);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

    s32 offset = 0;
    for (s32 i = 0; i < 96; i++) {
        // Unfortunately we still need to load the character again, as we need its bitmap buffer for the upload
        if (FT_Load_Char(face, i + 32, FT_LOAD_RENDER)) {
            continue;
        }
        glyph_info_t* info = (cache->info + i);
        info->texture_offset = (f32) offset / (f32) size.x;
        info->texture_span.x = info->size.x / (f32) size.x;
        info->texture_span.y = info->size.y / (f32) size.y;
        info->bearing.y -= (f32) size.y - info->size.y;
        glTexSubImage2D(GL_TEXTURE_2D, 0, offset, 0, (s32) info->size.x, (s32) info->size.y, GL_RED, GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer);
        offset += (s32) info->size.x;
    }

    free(font_data.data);
    font_data.size = 0;
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return true;
}

void glyph_cache_acquire(glyph_cache_t* cache, glyph_info_t* info, char symbol) {
    const glyph_info_t* fetched = (cache->info + symbol - 32);
    info->size = fetched->size;
    info->advance = fetched->advance;
    info->bearing = fetched->bearing;
    info->texture_span = fetched->texture_span;
    info->texture_offset = fetched->texture_offset;
}