// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_GPU_GLYPH_H
#define RETRO_GPU_GLYPH_H

enum {
    FONT_SIZE = 48
};

typedef struct GlyphInfo {
    S32Vector2 size;
    S32Vector2 bearing;
    S32Vector2 advance;
    F32Vector2 texture_span;
    f32 texture_offset;
} GlyphInfo;

typedef struct GlyphCache {
    Texture atlas;
    GlyphInfo info[128];
} GlyphCache;

/// Creates a glyph cache for the specified font
/// @param path The path to the TrueType font file
/// @return A new glyph cache
GlyphCache *glyph_cache_new(const char *path);

/// Destroys the glyph cache and its glyph atlas
/// @param self The glyph cache handle
void glyph_cache_free(GlyphCache *self);

/// Fetches the specified symbol from the glyph cache
/// @param self The glyph cache handle
/// @param info The glyph info handle where the data is placed into
/// @param symbol The symbol that shall be fetched
void glyph_cache_acquire(GlyphCache const *self, GlyphInfo *info, char symbol);

#endif// RETRO_GPU_GLYPH_H
