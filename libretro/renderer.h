#ifndef RETRO_RENDERER_H
#define RETRO_RENDERER_H

#include <string.h>

#include "buffer.h"
#include "glyph.h"
#include "shader.h"
#include "state.h"

#define QUAD_VERTICES 4
#define QUAD_INDICES 6
#define BATCH_CAPACITY 1024

typedef struct renderer {
    shader_t shader;
    vertex_array_t vertex_array;
    vertex_buffer_t vertex_buffer;
    index_buffer_t index_buffer;
    vertex_t vertices[BATCH_CAPACITY * QUAD_VERTICES];
    u32 indices[BATCH_CAPACITY * QUAD_INDICES];
    u32 batch_size;
} renderer_t;

/**
 * @brief clears the currently bound frame buffer
 */
void renderer_clear(void);

/**
 * @brief sets the clear color
 * @param color color value
 */
void renderer_clear_color(f32vec4_t* color);

/**
 * @brief creates a new renderer and initializes its pipeline
 * @param self renderer handle
 * @param shader shader handle
 */
void renderer_create(renderer_t* self, shader_t* shader);

/**
 * @brief destroys the specified renderer
 * @param self renderer handle
 */
void renderer_destroy(renderer_t* self);

/**
 * @brief begins a renderer batch
 * @param self renderer handle
 */
void renderer_begin_batch(renderer_t* self);

/**
 * @brief ends a renderer batch
 * @param self renderer handle
 */
void renderer_end_batch(renderer_t* self);

/**
 * @brief draws a quad using the specified renderer
 * @param self renderer handle
 * @param position position where the quad should start at
 * @param size size of the quad
 * @param color color of the quad
 */
void renderer_draw_quad(renderer_t* self, f32vec2_t* position, f32vec2_t* size, f32vec3_t* color);

typedef struct text_renderer {
    renderer_t renderer;
    glyph_cache_t cache;
} text_renderer_t;

/**
 * @brief creates a new text renderer and a glyph cache for it
 * @param self text renderer handle
 * @param path path to the .ttf file of the font
 * @return bool
 */
bool text_renderer_create(text_renderer_t* self, const char* path);

/**
 * @brief destroys the text renderer
 * @param self text renderer handle
 */
void text_renderer_destroy(text_renderer_t* self);

/**
 * @brief draws the specified symbol at the given position
 * @param self text renderer handle
 * @param symbol symbol that shall be drawn
 * @param position position were the symbol shall be drawn
 * @param color color for the symbol
 * @param scale scale of the text
 */
void text_renderer_draw_symbol(text_renderer_t* self,
                               glyph_info_t* symbol,
                               f32vec2_t* position,
                               f32vec3_t* color,
                               f32 scale);

/**
 * @brief draws the specified text at the given position
 * @param self text renderer handle
 * @param text text that shall be drawn
 * @param length length of the text
 * @param position position were the text shall be drawn
 * @param color color for the text
 * @param scale scale of the text
 */
void text_renderer_draw_text(text_renderer_t* self,
                             const char* text,
                             u32 length,
                             f32vec2_t* position,
                             f32vec3_t* color,
                             f32 scale);

/**
 * @brief draws the specified input buffer using a input indicator and cursor
 * @param self text renderer handle
 * @param input input buffer
 * @param position position were the text shall be drawn
 * @param color color for the text
 * @param scale scale of the text
 */
void text_renderer_draw_input(text_renderer_t* self,
                              input_buffer_t* input,
                              f32vec2_t* position,
                              f32vec3_t* color,
                              f32 scale);

#endif// RETRO_RENDERER_H