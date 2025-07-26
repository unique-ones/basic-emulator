// Copyright (c) 2025 Elias Engelbert Plank

#include <glad/glad.h>
#include <stddef.h>

#include "glyph.h"
#include "util/utility.h"

// clang-format off
#include <assert.h>
#include <ft2build.h>
#include <freetype/freetype.h>
// clang-format on

/// Creates a glyph cache for the specified font
GlyphCache *glyph_cache_new(char const *path) {
    GlyphCache *self = malloc(sizeof(GlyphCache));

    BinaryBuffer font_data;
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
    if (FT_New_Memory_Face(library, (FT_Byte *) font_data.data, font_data.size, 0, &face)) {
        free(font_data.data);
        font_data.size = 0;
        FT_Done_FreeType(library);
        return false;
    }
    FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);

    // calculate combined size of glyphs
    S32Vector2 size = { 0, 0 };
    for (u32 i = 32; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "could not load character: %c\n", (char) i);
            continue;
        }
        GlyphInfo *info = (self->info + i - 32);
        info->size.x = (s32) face->glyph->bitmap.width;
        info->size.y = (s32) face->glyph->bitmap.rows;
        info->bearing.x = face->glyph->bitmap_left;
        info->bearing.y = face->glyph->bitmap_top;
        info->advance.x = (s32) face->glyph->advance.x >> 6;
        info->advance.y = (s32) face->glyph->advance.y >> 6;
        info->texture_span.x = 0.0f;
        info->texture_span.y = 0.0f;
        info->texture_offset = 0.0f;
        size.x += info->advance.x;
        size.y = s32_max(size.y, info->size.y);
    }

    self->atlas.data = NULL;
    self->atlas.handle = 0;
    self->atlas.width = size.x;
    self->atlas.height = size.y;
    self->atlas.channels = 1;

    glGenTextures(1, &self->atlas.handle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, self->atlas.handle);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    s32 const length = size.x * size.y;
    u8 *data = malloc(length);
    if (data == NULL) {
        assert(false && "Unable to allocate data for blank texture!");
    }
    memset(data, 0, length);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    free(data);

    GLint const swizzle[] = { GL_ZERO, GL_ZERO, GL_ZERO, GL_RED };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);

    s32 offset = 0;
    for (u32 i = 0; i < 96; i++) {
        // unfortunately we still need to load the character again, as we need its bitmap buffer for the upload
        if (FT_Load_Char(face, i + 32, FT_LOAD_RENDER) || face->glyph->bitmap.buffer == NULL) {
            continue;
        }
        GlyphInfo *info = self->info + i;
        info->texture_offset = (f32) offset / (f32) size.x;
        info->texture_span.x = (f32) info->size.x / (f32) size.x;
        info->texture_span.y = (f32) info->size.y / (f32) size.y;
        info->bearing.y -= size.y - info->size.y;
        glTexSubImage2D(GL_TEXTURE_2D, 0, offset, 0, info->size.x, info->size.y, GL_RED, GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer);
        offset += info->advance.x;
    }

    free(font_data.data);
    font_data.size = 0;
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return self;
}

/// Destroys the glyph cache and its glyph atlas
void glyph_cache_free(GlyphCache *self) {
    texture_destroy(&self->atlas);
    free(self);
}

/// Fetches the specified symbol from the glyph cache
void glyph_cache_acquire(GlyphCache const *self, GlyphInfo *info, char const symbol) {
    *info = *(self->info + symbol - 32);
}
