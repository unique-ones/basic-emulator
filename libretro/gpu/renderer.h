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

#ifndef RETRO_GPU_RENDERER_H
#define RETRO_GPU_RENDERER_H

#include "arch/thread.h"
#include "buffer.h"
#include "glyph.h"
#include "shader.h"

enum { QUAD_VERTICES = 4, QUAD_INDICES = 6 };

/// Forward declare render command
typedef struct render_command render_command_t;

/// A render command enables lazy drawing, i.e. submitting draw data
/// without rendering it immediately. It is grouped together by a so
/// called render group, and sent to the GPU in one batch when necessary.
///
/// Due to the render group being a linked list, each render command
/// stores the previous and next command.
typedef struct render_command {
    render_command_t* prev;
    render_command_t* next;
    vertex_t vertices[QUAD_VERTICES];
    u32 indices[QUAD_INDICES];
} render_command_t;

/// Creates a new render command
/// @param vertices A list of vertices, must be exactly 4
/// @param indices A list of indices, must be exactly 6
/// @return A new render command instance
render_command_t* render_command_new(vertex_t* vertices, u32* indices);

/// Frees the specified render command instance
/// @param self The render command
void render_command_free(render_command_t* self);

enum { RENDER_GROUP_COMMANDS_MAX = 512 };

/// A render group enables the drawing of data, while synchronizing
/// state in order to behave safe in a concurrent context. Each render
/// group consists of a linked list of render commands, with a render
/// command being one draw call. In the context of the basic emulator,
/// a draw call is usually a quad or a glyph.
typedef struct render_group {
    // linked list of render commands
    render_command_t* begin;
    render_command_t* end;
    u32 commands;

    // drawing data
    vertex_array_t vertex_array;
    vertex_buffer_t vertex_buffer;
    index_buffer_t index_buffer;

    // synchronisation
    mutex_t* mutex;
} render_group_t;

/// Creates a new render group
/// @return A new render group
render_group_t* render_group_new(void);

/// Clears the specified render group (i.e. deletes the commands)
/// @param self The render group handle
void render_group_clear(render_group_t* self);

/// Frees the specified render group (i.e. delete the commands and free memory)
/// @param self The render group handle
void render_group_free(render_group_t* self);

/// Pushes a set of vertices and indices to the render group
/// @note blocks while there are too many commands in the render group
/// @param self The render group handle
/// @param vertices The vertex data, must be exactly 4
/// @param indices The indices data, must be exactly 6
void render_group_push(render_group_t* self, vertex_t* vertices, u32* indices);

enum { BLOOM_MIPS = 6 };

typedef struct post_processing {
    // The frame is the actual post processing result
    frame_buffer_t result;

    // The mips
    frame_buffer_t mips[BLOOM_MIPS];

    // The shader that does downsampling
    shader_t downsample_shader;

    /// The shader that does upsampling
    shader_t upsample_shader;

    /// The shader that blends the post processing effects together
    shader_t blending_shader;

    /// The post render group is responsible for post processing
    /// draw calls
    render_group_t* group;
} post_processing_t;

/// Creates the post processing pipeline
/// @param self The post processing handle
void post_processing_create(post_processing_t* self);

/// Destroys the post processing pipeline
/// @param self The post processing handle
void post_processing_destroy(post_processing_t* self);

typedef struct renderer {
    /// The glyph shader contains the logic to render a glyph using the
    /// unified glyph atlas
    shader_t glyph_shader;

    /// The glyph group holds all glyph render commands, therefore
    /// all text that is submitted to the GPU
    render_group_t* glyph_group;

    /// All the glyphs
    glyph_cache_t* glyphs;

    /// The quad shader contains the logic to render a quad
    shader_t quad_shader;

    /// The quad group holds all the quad render commands
    render_group_t* quad_group;

    /// The capture frame buffer
    frame_buffer_t capture;

    /// All post processing related things
    post_processing_t post;
} renderer_t;

/// Clears the currently bound frame buffer
void renderer_clear(void);

/// Sets the clear color
/// @param color The color value for clear calls
void renderer_clear_color(f32vec4_t* color);

/// Creates a new renderer and initializes its pipeline
/// @param self The renderer handle
/// @param font The font path
void renderer_create(renderer_t* self, const char* font);

/// Destroys the specified renderer
/// @param self The renderer handle
void renderer_destroy(renderer_t* self);

/// Begins a renderer batch by resetting all render groups
/// @param self The renderer handle
void renderer_begin_batch(renderer_t* self);

/// Ends a renderer batch by submitting the commands of all render groups
/// @param self The renderer handle
void renderer_end_batch(renderer_t* self);

/// Indicate to the renderer that a resize is necessary
/// @param self The renderer handle
/// @param width The new width
/// @param height The new height
void renderer_resize(renderer_t* self, s32 width, s32 height);

/// Draws a quad at the given position
/// @param self The renderer handle
/// @param position The position where the quad shall be drawn
/// @param size The size of the quad
/// @param color The color of the quad
void renderer_draw_quad(renderer_t* self, f32vec2_t* position, f32vec2_t* size, f32vec3_t* color);

/// Draws the specified symbol at the given position
/// @param self The renderer handle
/// @param symbol The symbol that shall be drawn
/// @param position The position were the symbol shall be drawn
/// @param color The color for the symbol
/// @param scale The scale of the text
void renderer_draw_symbol(renderer_t* self, glyph_info_t* symbol, f32vec2_t* position, f32vec3_t* color, f32 scale);

/// Draws the specified text at the given position
/// @param self The renderer handle
/// @param position The position were the text shall be drawn
/// @param color The color for the text
/// @param scale The scale of the text
/// @param fmt The format text that shall be drawn
/// @param ... The variadic arguments
void renderer_draw_text(renderer_t* self, f32vec2_t* position, f32vec3_t* color, f32 scale, const char* fmt, ...);

/// Draws the cursor and the specified text at the given position
/// @param self The renderer handle
/// @param position The position where the text shall be drawn
/// @param color The color for the text
/// @param scale The scale of the text
/// @param cursor_index The index of the cursor
/// @param fmt The format text that shall be drawn
/// @param ... The variadic arguments
void renderer_draw_text_with_cursor(renderer_t* self,
                                    f32vec2_t* position,
                                    f32vec3_t* color,
                                    f32 scale,
                                    u32 cursor_index,
                                    const char* fmt,
                                    ...);

/// Captures all following draw commands into a frame buffer
/// @param self The renderer handle
void renderer_begin_capture(renderer_t* self);

/// Ends the capture of draw commands
/// @param self The renderer handle
void renderer_end_capture(renderer_t* self);

#endif// RETRO_GPU_RENDERER_H
