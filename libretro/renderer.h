#ifndef RETRO_RENDERER_H
#define RETRO_RENDERER_H

#include <string.h>

#include "arch/thread.h"
#include "buffer.h"
#include "glyph.h"
#include "shader.h"
#include "text.h"

enum {
    QUAD_VERTICES = 4,
    QUAD_INDICES = 6
};

typedef struct render_command {
    struct render_command* prev;
    struct render_command* next;
    vertex_t vertices[QUAD_VERTICES];
    u32 indices[QUAD_INDICES];
} render_command_t;

/**
 * @brief create a new render command
 * @param vertices list of vertices, must be exactly 4
 * @param indices list of indices, must be exactly 6
 * @return render command instance
 */
render_command_t* render_command_new(vertex_t* vertices, u32* indices);

/**
 * @brief free the specified render command instance
 * @param self render command
 */
void render_command_free(render_command_t* self);

enum {
    RENDER_GROUP_COMMANDS_MAX = 512
};

typedef struct render_group {
    render_command_t* begin;
    render_command_t* end;
    u32 commands;
    mutex_t* mutex;
} render_group_t;

/**
 * @brief create a new render group
 * @return render group
 */
render_group_t* render_group_new(void);

/**
 * @brief clear the specified render group (i.e. delete the commands)
 * @param self render group
 */
void render_group_clear(render_group_t* self);

/**
 * @brief free the specified render group (i.e. delete the commands and free memory)
 * @param self render group
 */
void render_group_free(render_group_t* self);

/**
 * @brief push a set of vertices and indices to the render group
 * @note blocks while there are too many commands in the render group
 * @param self render group
 */
void render_group_push(render_group_t* self, vertex_t* vertices, u32* indices);

typedef struct renderer {
    vertex_array_t vertex_array;
    vertex_buffer_t vertex_buffer;
    index_buffer_t index_buffer;

    shader_t glyph_shader;
    render_group_t* glyph_group;
    glyph_cache_t* glyphs;

    shader_t quad_shader;
    render_group_t* quad_group;
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
 * @param font font handle
 */
void renderer_create(renderer_t* self, const char* font);

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
 * @brief draws a quad at the given position
 * @param self renderer handle
 * @param position position where the quad shall be drawn
 * @param size size of the quad
 * @param color color of the quad
 */
void renderer_draw_quad(renderer_t* self, f32vec2_t* position, f32vec2_t* size, f32vec3_t* color);

/**
 * @brief draws the specified symbol at the given position
 * @param self renderer handle
 * @param symbol symbol that shall be drawn
 * @param position position were the symbol shall be drawn
 * @param color color for the symbol
 * @param scale scale of the text
 */
void renderer_draw_symbol(renderer_t* self, glyph_info_t* symbol, f32vec2_t* position, f32vec3_t* color, f32 scale);

/**
 * @brief draws the specified text at the given position
 * @param self renderer handle
 * @param length length of the text
 * @param position position were the text shall be drawn
 * @param color color for the text
 * @param scale scale of the text
 * @param fmt text that shall be drawn
 */
void renderer_draw_text(renderer_t* self, f32vec2_t* position, f32vec3_t* color, f32 scale, const char* fmt, ...);


void renderer_draw_text_with_cursor(renderer_t* self,
                                    f32vec2_t* position,
                                    f32vec3_t* color,
                                    f32 scale,
                                    u32 cursor_index,
                                    const char* fmt,
                                    ...);

#endif// RETRO_RENDERER_H
