#ifndef RETRO_RENDERER_H
#define RETRO_RENDERER_H

#include "buffer.h"
#include "glyph.h"
#include "shader.h"


#include <string.h>

#define MAX_BATCH_SIZE 1024

typedef struct renderer {
    shader_t shader;
    vertex_array_t vertex_array;
    vertex_buffer_t vertex_buffer;
    index_buffer_t index_buffer;
    vertex_t vertices[MAX_BATCH_SIZE * 4];
    u32 indices[MAX_BATCH_SIZE * 6];
    u32 batch_size;
} renderer_t;

/**
 * @brief Clears the currently bound frame buffer
 */
void renderer_clear(void);

/**
 * @brief Sets the clear color
 * @param color Color value
 */
void renderer_clear_color(const f32vec4_t* color);

/**
 * @brief Creates a new renderer and initializes its pipeline
 * @param renderer Renderer handle
 * @param shader Optional shader handle, if NULL is passed, a default shader is used
 */
void renderer_create(renderer_t* renderer, shader_t* shader);

/**
 * @brief Destroys the specified renderer
 * @param renderer Renderer handle
 */
void renderer_destroy(renderer_t* renderer);

/**
 * @brief Begins a renderer batch
 * @param renderer Renderer
 */
void renderer_begin_batch(renderer_t* renderer);

/**
 * @brief Ends a renderer batch
 * @param renderer Renderer
 */
void renderer_end_batch(renderer_t* renderer);

/**
 * @brief Draws a quad using the specified renderer and the other parameters
 * @param renderer Renderer handle
 * @param position Position where the quad should start at
 * @param size Size of the quad
 * @param color Color of the quad
 */
void renderer_draw_quad(renderer_t* renderer, const f32vec2_t* position, const f32vec2_t* size, const f32vec3_t* color);

typedef struct text_renderer {
    renderer_t renderer;
    glyph_cache_t cache;
} text_renderer_t;

/**
 * @brief Creates a new text renderer and a glyph cache for it
 * @param text_renderer Text renderer handle
 * @param path Path to the .ttf file of the font
 * @return bool
 */
bool text_renderer_create(text_renderer_t* text_renderer, const char* path);

/**
 * @brief Draws the specified symbol at the given position
 * @param text_renderer Text renderer handle
 * @param symbol Symbol that shall be drawn
 * @param position Position were the symbol shall be drawn
 * @param color Color for the symbol
 * @param scale Scale of the text
 */
void text_renderer_draw_symbol(text_renderer_t* text_renderer,
                               glyph_info_t* symbol,
                               const f32vec2_t* position,
                               const f32vec3_t* color,
                               f32 scale);

/**
 * @brief Draws the specified text at the given position
 * @param text_renderer Text renderer handle
 * @param text Text that shall be drawn
 * @param position Position were the text shall be drawn
 * @param color Color for the text
 * @param scale Scale of the text
 */
void text_renderer_draw_text(text_renderer_t* text_renderer,
                             const char* text,
                             const f32vec2_t* position,
                             const f32vec3_t* color,
                             f32 scale);

#endif// RETRO_RENDERER_H