// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_GPU_RENDERER_H
#define RETRO_GPU_RENDERER_H

#include "arch/thread.h"
#include "buffer.h"
#include "glyph.h"
#include "shader.h"

enum {
    QUAD_VERTICES = 4,
    QUAD_INDICES = 6
};

/// Forward declare render command
typedef struct RenderCommand RenderCommand;

/// A render command enables lazy drawing, i.e. submitting draw data
/// without rendering it immediately. It is grouped together by a so-
/// called render group, and sent to the GPU in one batch when necessary.
///
/// Due to the render group being a linked list, each render command
/// stores the previous and next command.
typedef struct RenderCommand {
    RenderCommand *prev;
    RenderCommand *next;
    Vertex vertices[QUAD_VERTICES];
    u32 indices[QUAD_INDICES];
} RenderCommand;

/// Creates a new render command
/// @param vertices A list of vertices, must be exactly 4
/// @param indices A list of indices, must be exactly 6
/// @return A new render command instance
RenderCommand *render_command_new(Vertex const *vertices, u32 const *indices);

/// Frees the specified render command instance
/// @param self The render command
void render_command_free(RenderCommand *self);

enum {
    RENDER_GROUP_COMMANDS_MAX = 512
};

/// A render group enables the drawing of data, while synchronizing
/// state in order to behave safe in a concurrent context. Each render
/// group consists of a linked list of render commands, with a render
/// command being one draw call. In the context of the basic emulator,
/// a draw call is usually a quad or a glyph.
typedef struct RenderGroup {
    // linked list of render commands
    RenderCommand *begin;
    RenderCommand *end;
    u32 commands;

    // drawing data
    VertexArray vertex_array;
    VertexBuffer vertex_buffer;
    IndexBuffer index_buffer;

    // synchronisation
    Mutex *mutex;
} RenderGroup;

/// Creates a new render group
/// @return A new render group
RenderGroup *render_group_new(void);

/// Clears the specified render group (i.e. deletes the commands)
/// @param self The render group handle
void render_group_clear(RenderGroup *self);

/// Frees the specified render group (i.e. delete the commands and free memory)
/// @param self The render group handle
void render_group_free(RenderGroup *self);

/// Pushes a set of vertices and indices to the render group
/// @note blocks while there are too many commands in the render group
/// @param self The render group handle
/// @param vertices The vertex data, must be exactly 4
/// @param indices The indices data, must be exactly 6
void render_group_push(RenderGroup *self, Vertex const *vertices, u32 const *indices);

enum {
    BLOOM_MIPS = 6
};

typedef struct PostProcessing {
    // The frame is the actual post-processing result
    FrameBuffer result;

    // The mips
    FrameBuffer mips[BLOOM_MIPS];

    // The shader that does down-sampling
    Shader downsample_shader;

    /// The shader that does up-sampling
    Shader upsample_shader;

    /// The shader that blends the post-processing effects together
    Shader blending_shader;

    /// The post render group is responsible for post-processing
    /// draw calls
    RenderGroup *group;
} PostProcessing;

/// Creates the post-processing pipeline
/// @param self The post-processing handle
void post_processing_create(PostProcessing *self);

/// Destroys the post-processing pipeline
/// @param self The post-processing handle
void post_processing_destroy(PostProcessing const *self);

typedef struct Renderer {
    /// The glyph shader contains the logic to render a glyph using the
    /// unified glyph atlas
    Shader glyph_shader;

    /// The glyph group holds all glyph render commands, therefore
    /// all text that is submitted to the GPU
    RenderGroup *glyph_group;

    /// All the glyphs
    GlyphCache *glyphs;

    /// The quad shader contains the logic to render a quad
    Shader quad_shader;

    /// The quad group holds all the quad render commands
    RenderGroup *quad_group;

    /// The capture frame buffer
    FrameBuffer capture;

    /// All post-processing related things
    PostProcessing post;
} Renderer;

/// Clears the currently bound frame buffer
void renderer_clear(void);

/// Sets the clear color
/// @param color The color value for clear calls
void renderer_clear_color(F32Vector4 const *color);

/// Creates a new renderer and initializes its pipeline
/// @param self The renderer handle
/// @param font The font path
void renderer_create(Renderer *self, const char *font);

/// Destroys the specified renderer
/// @param self The renderer handle
void renderer_destroy(Renderer const *self);

/// Begins a renderer batch by resetting all render groups
/// @param self The renderer handle
void renderer_begin_batch(Renderer const *self);

/// Ends a renderer batch by submitting the commands of all render groups
/// @param self The renderer handle
void renderer_end_batch(Renderer const *self);

/// Indicate to the renderer that a resize is necessary
/// @param self The renderer handle
/// @param width The new width
/// @param height The new height
void renderer_resize(Renderer *self, s32 width, s32 height);

/// Draws a quad at the given position
/// @param self The renderer handle
/// @param position The position where the quad shall be drawn
/// @param size The size of the quad
/// @param color The color of the quad
void renderer_draw_quad(Renderer const *self,
                        F32Vector2 const *position,
                        F32Vector2 const *size,
                        F32Vector3 const *color);

/// Draws the specified symbol at the given position
/// @param self The renderer handle
/// @param symbol The symbol that shall be drawn
/// @param position The position were the symbol shall be drawn
/// @param color The color for the symbol
/// @param scale The scale of the text
void renderer_draw_symbol(Renderer const *self,
                          GlyphInfo const *symbol,
                          F32Vector2 const *position,
                          F32Vector3 const *color,
                          f32 scale);

/// Draws the specified text at the given position
/// @param self The renderer handle
/// @param position The position were the text shall be drawn
/// @param color The color for the text
/// @param scale The scale of the text
/// @param fmt The format text that shall be drawn
/// @param ... The variadic arguments
void renderer_draw_text(Renderer const *self,
                        F32Vector2 *position,
                        F32Vector3 const *color,
                        f32 scale,
                        const char *fmt,
                        ...);

/// Draws the cursor and the specified text at the given position
/// @param self The renderer handle
/// @param position The position where the text shall be drawn
/// @param color The color for the text
/// @param scale The scale of the text
/// @param cursor_index The index of the cursor
/// @param fmt The format text that shall be drawn
/// @param ... The variadic arguments
void renderer_draw_text_with_cursor(Renderer const *self,
                                    F32Vector2 *position,
                                    F32Vector3 const *color,
                                    f32 scale,
                                    u32 cursor_index,
                                    const char *fmt,
                                    ...);

/// Captures all following draw commands into a frame buffer
/// @param self The renderer handle
void renderer_begin_capture(Renderer const *self);

/// Ends the capture of draw commands
/// @param self The renderer handle
void renderer_end_capture(Renderer const *self);

#endif// RETRO_GPU_RENDERER_H
