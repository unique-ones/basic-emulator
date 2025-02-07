// Copyright (c) 2025 Elias Engelbert Plank

#include <glad/glad.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "renderer.h"

/// Creates a new render command
RenderCommand *render_command_new(Vertex const *vertices, u32 const *indices) {
    RenderCommand *self = malloc(sizeof(RenderCommand));
    self->prev = NULL;
    self->next = NULL;
    memcpy(self->vertices, vertices, sizeof self->vertices);
    memcpy(self->indices, indices, sizeof self->indices);
    return self;
}

/// Frees the specified render command instance
void render_command_free(RenderCommand *self) {
    free(self);
}

/// Creates a new render group
RenderGroup *render_group_new(void) {
    RenderGroup *self = malloc(sizeof(RenderGroup));
    self->begin = NULL;
    self->end = NULL;
    self->commands = 0;
    self->mutex = mutex_new();

    vertex_array_create(&self->vertex_array);
    vertex_buffer_create(&self->vertex_buffer);
    index_buffer_create(&self->index_buffer);

    // attributes are usually `attrib_position`, `attrib_color`, `attrib_texture`
    static ShaderType attributes[] = { FLOAT3, FLOAT3, FLOAT2 };
    static VertexBufferLayout layout;
    layout.attributes = attributes;
    layout.count = STACK_ARRAY_SIZE(attributes);

    vertex_buffer_layout(&self->vertex_buffer, &layout);
    vertex_array_vertex_buffer(&self->vertex_array, &self->vertex_buffer);
    vertex_array_index_buffer(&self->vertex_array, &self->index_buffer);
    return self;
}

/// Clears the specified render group (i.e. deletes the commands)
void render_group_clear(RenderGroup *self) {
    mutex_lock(self->mutex);
    RenderCommand *it = self->begin;

    while (it != NULL) {
        RenderCommand *tmp = it;
        it = it->next;
        render_command_free(tmp);
    }

    self->begin = NULL;
    self->end = NULL;
    self->commands = 0;
    mutex_unlock(self->mutex);
}

/// Frees the specified render group (i.e. delete the commands and free memory)
void render_group_free(RenderGroup *self) {
    render_group_clear(self);
    mutex_free(self->mutex);
    index_buffer_destroy(&self->index_buffer);
    vertex_buffer_destroy(&self->vertex_buffer);
    vertex_array_destroy(&self->vertex_array);
    free(self);
}

/// Pushes a set of vertices and indices to the render group
void render_group_push(RenderGroup *self, Vertex const *vertices, u32 const *indices) {
    while (self->commands >= RENDER_GROUP_COMMANDS_MAX)
        ;

    mutex_lock(self->mutex);
    RenderCommand *command = render_command_new(vertices, indices);
    if (self->begin == NULL) {
        self->begin = command;
        self->commands++;
        mutex_unlock(self->mutex);
        return;
    }

    RenderCommand *tmp = self->begin;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = command;
    command->prev = tmp;
    self->end = command;
    self->commands++;
    mutex_unlock(self->mutex);
}

/// Creates the post-processing pipeline
void post_processing_create(PostProcessing *self) {
    FrameBufferSpecification spec = { .width = 800,
                                      .height = 600,
                                      .internal_format = GL_RGBA16F,
                                      .pixel_type = GL_FLOAT,
                                      .pixel_format = GL_RGB };

    frame_buffer_create(&self->result, &spec);

    for (u32 i = 0; i < BLOOM_MIPS; ++i) {
        frame_buffer_create(self->mips + i, &spec);
        spec.width /= 2;
        spec.height /= 2;
    }

    self->group = render_group_new();
    shader_create(&self->downsample_shader, "assets/vertex.glsl", "assets/bloom_downsample_fragment.glsl");
    shader_create(&self->upsample_shader, "assets/vertex.glsl", "assets/bloom_upsample_fragment.glsl");
    shader_create(&self->blending_shader, "assets/vertex.glsl", "assets/bloom_blending_fragment.glsl");
}

/// Destroys the post-processing pipeline
void post_processing_destroy(PostProcessing const *self) {
    shader_destroy(&self->downsample_shader);
    shader_destroy(&self->upsample_shader);
    shader_destroy(&self->blending_shader);

    render_group_free(self->group);

    for (u32 i = 0; i < BLOOM_MIPS; ++i) {
        frame_buffer_destroy(self->mips + i);
    }

    frame_buffer_destroy(&self->result);
}

/// Submits an actual indexed OpenGL draw call to the GPU
static void renderer_draw_indexed(VertexArray const *vertex_array, Shader const *shader, u32 const mode) {
    vertex_array_bind(vertex_array);
    shader_bind(shader);
    glDrawElements(mode, (s32) vertex_array->index_buffer->count, GL_UNSIGNED_INT, NULL);
    vertex_array_unbind();
}

/// Clears the currently bound frame buffer
void renderer_clear(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// Sets the clear color
void renderer_clear_color(F32Vector4 const *color) {
    glClearColor(color->x, color->y, color->z, color->w);
}

/// Creates a new renderer and initializes its pipeline
void renderer_create(Renderer *self, const char *font) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_create(&self->glyph_shader, "assets/vertex.glsl", "assets/glyph_fragment.glsl");
    self->glyph_group = render_group_new();
    self->glyphs = glyph_cache_new(font);

    shader_create(&self->quad_shader, "assets/vertex.glsl", "assets/quad_fragment.glsl");
    self->quad_group = render_group_new();

    FrameBufferSpecification const spec = { .width = 800,
                                            .height = 600,
                                            .internal_format = GL_RGBA16F,
                                            .pixel_type = GL_FLOAT,
                                            .pixel_format = GL_RGB };

    frame_buffer_create(&self->capture, &spec);
    post_processing_create(&self->post);
}

/// Destroys the specified renderer
void renderer_destroy(Renderer const *self) {
    shader_destroy(&self->glyph_shader);
    render_group_free(self->glyph_group);
    glyph_cache_free(self->glyphs);
    shader_destroy(&self->quad_shader);
    render_group_free(self->quad_group);
    frame_buffer_destroy(&self->capture);
    post_processing_destroy(&self->post);
}

/// Begins a renderer batch by resetting all render groups
void renderer_begin_batch(Renderer const *self) {
    render_group_clear(self->glyph_group);
    render_group_clear(self->quad_group);
}

/// Submits the specified render group and issues an indexed draw call
static void render_group_submit(RenderGroup *group, Shader const *shader) {
    if (group->commands == 0) {
        return;
    }

    mutex_lock(group->mutex);
    u32 const vertices_size = QUAD_VERTICES * sizeof(Vertex);
    u32 const indices_size = QUAD_INDICES * sizeof(u32);
    Vertex *vertices = malloc((u64) vertices_size * group->commands);
    u32 *indices = malloc((u64) indices_size * group->commands);

    u32 insert_index = 0;
    for (RenderCommand const *it = group->begin; it != NULL; it = it->next) {
        memcpy((u8 *) vertices + (ptrdiff_t) (vertices_size * insert_index), it->vertices, vertices_size);
        memcpy((u8 *) indices + (ptrdiff_t) (indices_size * insert_index), it->indices, indices_size);
        insert_index++;
    }

    vertex_buffer_data(&group->vertex_buffer, vertices, group->commands * vertices_size);
    free(vertices);

    index_buffer_data(&group->index_buffer, indices, group->commands * QUAD_INDICES);
    free(indices);

    renderer_draw_indexed(&group->vertex_array, shader, GL_TRIANGLES);
    mutex_unlock(group->mutex);
}

/// Ends a renderer batch by submitting the commands of all render groups
void renderer_end_batch(Renderer const *self) {
    render_group_submit(self->quad_group, &self->quad_shader);

    texture_bind(&self->glyphs->atlas, 0);
    shader_uniform_sampler(&self->glyph_shader, "uniform_glyph_atlas", 0);
    render_group_submit(self->glyph_group, &self->glyph_shader);
}

/// Indicate to the renderer that a resize is necessary
void renderer_resize(Renderer *self, s32 const width, s32 const height) {
    F32Mat4 orthogonal;
    f32mat4_create_orthogonal(&orthogonal, 0.0f, (f32) width, (f32) height, 0.0f);
    shader_uniform_f32mat4(&self->glyph_shader, "uniform_transform", &orthogonal);
    shader_uniform_f32mat4(&self->quad_shader, "uniform_transform", &orthogonal);
    shader_uniform_f32mat4(&self->post.downsample_shader, "uniform_transform", &orthogonal);
    shader_uniform_f32mat4(&self->post.upsample_shader, "uniform_transform", &orthogonal);
    shader_uniform_f32mat4(&self->post.blending_shader, "uniform_transform", &orthogonal);

    // frame buffer requires resize
    frame_buffer_resize(&self->capture, width, height);
    frame_buffer_resize(&self->post.result, width, height);
    for (u32 i = 0; i < BLOOM_MIPS; ++i) {
        frame_buffer_resize(self->post.mips + i, width, height);
    }
}

/// Draws a quad at the given position
void renderer_draw_quad(Renderer const *self,
                        F32Vector2 const *position,
                        F32Vector2 const *size,
                        F32Vector3 const *color) {
    Vertex const vertices[] = { { { position->x, position->y, 0.0f }, *color, { 0.0f, 1.0f } },
                                { { position->x, position->y + size->y, 0.0f }, *color, { 0.0f, 0.0f } },
                                { { position->x + size->x, position->y + size->y, 0.0f }, *color, { 1.0f, 0.0f } },
                                { { position->x + size->x, position->y, 0.0f }, *color, { 1.0f, 1.0f } } };

    u32 const index_offset = self->quad_group->commands * 4;
    u32 const indices[] = { 0 + index_offset, 1 + index_offset, 2 + index_offset,
                            2 + index_offset, 0 + index_offset, 3 + index_offset };
    render_group_push(self->quad_group, vertices, indices);
}

/// Draws the specified symbol at the given position
void renderer_draw_symbol(Renderer const *self,
                          GlyphInfo const *symbol,
                          F32Vector2 const *position,
                          F32Vector3 const *color,
                          f32 const scale) {
    // clang-format off
    F32Vector2 const scaled_size = { (f32) symbol->size.x * scale, (f32) symbol->size.y * scale };
    F32Vector2 const scaled_position = {
        position->x + (f32) symbol->bearing.x * scale,
        position->y + ((f32) symbol->size.y - (f32) symbol->bearing.y) * scale
    };
    Vertex const vertices[] = {
        { { scaled_position.x, scaled_position.y, 0.0f }, *color, { symbol->texture_offset, 0.0f } },
        { { scaled_position.x, scaled_position.y + scaled_size.y, 0.0f }, *color, { symbol->texture_offset, symbol->texture_span.y } },
        { { scaled_position.x + scaled_size.x, scaled_position.y + scaled_size.y, 0.0f }, *color, { symbol->texture_offset + symbol->texture_span.x, symbol->texture_span.y } },
        { { scaled_position.x + scaled_size.x, scaled_position.y, 0.0f }, *color, { symbol->texture_offset + symbol->texture_span.x, 0.0f } }
    };
    // clang-format on

    u32 const index_offset = self->glyph_group->commands * 4;
    u32 const indices[] = { 0 + index_offset, 1 + index_offset, 2 + index_offset,
                            2 + index_offset, 0 + index_offset, 3 + index_offset };
    render_group_push(self->glyph_group, vertices, indices);
}

/// Draws the specified text at the given position
void renderer_draw_text(Renderer const *self,
                        F32Vector2 *position,
                        F32Vector3 const *color,
                        f32 const scale,
                        const char *fmt,
                        ...) {
    char text_buffer[0x1000];
    va_list list;
    va_start(list, fmt);
    u32 const length = vsnprintf(text_buffer, sizeof text_buffer, fmt, list);
    va_end(list);

    F32Vector2 position_iterator = *position;
    for (u32 i = 0; i < length; i++) {
        char const symbol = text_buffer[i];
        if (symbol == '\n') {
            position_iterator.x = position->x;
            position_iterator.y += FONT_SIZE * scale;
        } else if (symbol == '\t') {
            GlyphInfo glyph_info;
            glyph_cache_acquire(self->glyphs, &glyph_info, ' ');
            for (u32 j = 0; j < 4; j++) {
                renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
                position_iterator.x += (f32) glyph_info.advance.x * scale;
            }
        } else {
            GlyphInfo glyph_info;
            glyph_cache_acquire(self->glyphs, &glyph_info, symbol);
            renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
            position_iterator.x += (f32) glyph_info.advance.x * scale;
        }
    }
    *position = position_iterator;
}

/// Draws an input indicator
static void renderer_draw_indicator(Renderer const *self,
                                    F32Vector2 *position,
                                    F32Vector3 const *color,
                                    f32 const scale) {
    // first we draw the input indicator
    GlyphInfo input_indicator_info;
    glyph_cache_acquire(self->glyphs, &input_indicator_info, ']');
    renderer_draw_symbol(self, &input_indicator_info, position, color, scale);
    position->x += (f32) input_indicator_info.advance.x * scale;
}

/// Draws the cursor and the specified text at the given position
void renderer_draw_text_with_cursor(Renderer const *self,
                                    F32Vector2 *position,
                                    F32Vector3 const *color,
                                    f32 const scale,
                                    u32 const cursor_index,
                                    const char *fmt,
                                    ...) {
    char text_buffer[0x1000];
    va_list list;
    va_start(list, fmt);
    u32 const length = vsnprintf(text_buffer, sizeof text_buffer, fmt, list);
    va_end(list);

    F32Vector2 position_iterator = *position;
    renderer_draw_indicator(self, &position_iterator, color, scale);

    // then we fetch the cursor glyph
    GlyphInfo cursor_info;
    glyph_cache_acquire(self->glyphs, &cursor_info, '_');
    if (cursor_index == 0) {
        renderer_draw_symbol(self, &cursor_info, &position_iterator, color, scale);
    }

    for (u32 i = 0; i < length; i++) {
        char const symbol = text_buffer[i];
        if (symbol == '\t') {
            // if we encounter a tab, advance by four spaces
            GlyphInfo glyph_info;
            glyph_cache_acquire(self->glyphs, &glyph_info, ' ');
            for (u32 j = 0; j < 4; j++) {
                renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
                position_iterator.x += (f32) glyph_info.advance.x * scale;
            }
        } else if (symbol == '\n') {
            position_iterator.x = position->x;
            position_iterator.y += FONT_SIZE * scale;
            renderer_draw_indicator(self, &position_iterator, color, scale);
        } else {
            // any other character
            GlyphInfo glyph_info;
            glyph_cache_acquire(self->glyphs, &glyph_info, symbol);
            renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
            position_iterator.x += (f32) glyph_info.advance.x * scale;
        }

        if (i == cursor_index - 1) {
            renderer_draw_symbol(self, &cursor_info, &position_iterator, color, scale);
        }
    }
    *position = position_iterator;
}

/// Captures all following draw commands into a frame buffer
void renderer_crt_begin_capture(Renderer const *self) {
    frame_buffer_bind(&self->capture);
}

/// Ends the capture of draw commands
void renderer_crt_end_capture(Renderer const *self) {
    // unbind frame buffer in order to actually render stuff now
    frame_buffer_unbind();
    render_group_clear(self->post.group);

    F32Vector2 const size = { (f32) self->post.result.spec.width, (f32) self->post.result.spec.height };
    F32Vector3 const color = { 1.0f, 1.0f, 1.0f };

    // clang-format off
    Vertex const vertices[] = {
        { .position = { 0.0f, 0.0f, 0.0f }, .color = color, { 0.0f, 1.0f } },
        { .position = { 0.0f, size.y, 0.0f }, .color = color, { 0.0f, 0.0f } },
        { .position = { size.x, size.y, 0.0f }, .color = color, { 1.0f, 0.0f } },
        { .position = { size.x, 0.0f, 0.0f }, .color = color, { 1.0f, 1.0f } }
    };
    // clang-format on
    u32 const indices[] = { 0, 1, 2, 2, 0, 3 };

    // prepare screen-sized vertices for render passes
    render_group_push(self->post.group, vertices, indices);

    // bind the capture frame buffer texture as a starting point
    // for the down samples
    frame_buffer_bind_texture(&self->capture, 0);

    // progressively downsample
    for (u32 i = 0; i < BLOOM_MIPS; ++i) {
        FrameBuffer const *mip = self->post.mips + i;

        frame_buffer_bind(mip);
        F32Vector2 resolution = { (f32) mip->spec.width, (f32) mip->spec.height };
        shader_uniform_sampler(&self->post.downsample_shader, "uniform_frame", 0);
        shader_uniform_f32vec2(&self->post.downsample_shader, "uniform_resolution", &resolution);

        glViewport(0, 0, mip->spec.width, mip->spec.height);
        render_group_submit(self->post.group, &self->post.downsample_shader);
        frame_buffer_unbind();

        // it is necessary to bind the current mip texture
        // as the input for the next down sampling pass
        frame_buffer_bind_texture(mip, 0);
    }

    // enable additive blending for the up-sample pass. contrary
    // to the downsample pass, where we have a separate texture
    // for each sample, we just render the progressive up-samples
    // to one target, which is self->post.result
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    // bind the result frame buffer for rendering
    frame_buffer_bind(&self->post.result);
    renderer_clear();

    for (u32 i = 0; i < BLOOM_MIPS; ++i) {
        FrameBuffer const *mip = self->post.mips + i;
        frame_buffer_bind_texture(mip, 0);
        shader_uniform_sampler(&self->post.downsample_shader, "uniform_frame", 0);
        shader_uniform_f32(&self->post.upsample_shader, "uniform_filter_radius", 1.0f);

        glViewport(0, 0, mip->spec.width, mip->spec.height);
        render_group_submit(self->post.group, &self->post.upsample_shader);
    }

    // unbind result frame buffer for final drawing to the screen
    frame_buffer_unbind();

    // reset viewport and blending mode to its original state
    glViewport(0, 0, self->post.result.spec.width, self->post.result.spec.height);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    F32Vector2 const curvature = { 4.0f, 4.0f };
    F32Vector2 const opacity = { 0.1f, 0.1f };

    frame_buffer_bind_texture(&self->capture, 0);
    frame_buffer_bind_texture(&self->post.result, 1);
    shader_uniform_sampler(&self->post.blending_shader, "uniform_capture", 0);
    shader_uniform_sampler(&self->post.blending_shader, "uniform_bloom", 1);
    shader_uniform_f32vec2(&self->post.blending_shader, "uniform_curvature", &curvature);
    shader_uniform_f32vec2(&self->post.blending_shader, "uniform_resolution", &size);
    shader_uniform_f32vec2(&self->post.blending_shader, "uniform_opacity", &opacity);
    shader_uniform_f32(&self->post.blending_shader, "uniform_vignette_opacity", 1.0f);
    shader_uniform_f32(&self->post.blending_shader, "uniform_vignette_roundness", 2.0f);
    shader_uniform_f32(&self->post.blending_shader, "uniform_brightness", 2.0f);
    render_group_submit(self->post.group, &self->post.blending_shader);
}
