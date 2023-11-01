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

#include "glyph.h"
#include "util/utility.h"

#include <ft2build.h>
#include FT_FREETYPE_H

/// Creates a glyph cache for the specified font
glyph_cache_t* glyph_cache_new(const char* path) {
    glyph_cache_t* self = (glyph_cache_t*) malloc(sizeof(glyph_cache_t));

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

    // calculate combined size of glyphs
    s32vec2_t size = { 0, 0 };
    for (s32 i = 32; i < 128; i++) {
        if (FT_Load_Char(face, (FT_ULong) i, FT_LOAD_RENDER)) {
            fprintf(stderr, "could not load character: %c\n", (char) i);
            continue;
        }
        glyph_info_t* info = (self->info + i - 32);
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
        size.y = s32_max(size.y, (s32) face->glyph->bitmap.rows);
    }

    self->atlas.data = NULL;
    self->atlas.handle = 0;
    self->atlas.width = size.x;
    self->atlas.height = size.y;
    self->atlas.channels = 1;
    glCreateTextures(GL_TEXTURE_2D, 1, &self->atlas.handle);
    glBindTextureUnit(0, self->atlas.handle);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

    s32 offset = 0;
    for (u32 i = 0; i < 96; i++) {
        // unfortunately we still need to load the character again, as we need its bitmap buffer for the upload
        if (FT_Load_Char(face, i + 32, FT_LOAD_RENDER) || face->glyph->bitmap.buffer == NULL) {
            continue;
        }
        glyph_info_t* info = (self->info + i);
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
    return self;
}

/// Destroys the glyph cache and its glyph atlas
void glyph_cache_free(glyph_cache_t* self) {
    texture_destroy(&self->atlas);
    free(self);
}

/// Fetches the specified symbol from the glyph cache
void glyph_cache_acquire(glyph_cache_t* self, glyph_info_t* info, char symbol) {
    glyph_info_t* fetched = (self->info + symbol - 32);
    info->size = fetched->size;
    info->advance = fetched->advance;
    info->bearing = fetched->bearing;
    info->texture_span = fetched->texture_span;
    info->texture_offset = fetched->texture_offset;
}
