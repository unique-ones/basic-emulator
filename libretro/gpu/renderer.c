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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "arch/time.h"
#include "renderer.h"

/// Creates a new render command
render_command_t* render_command_new(vertex_t* vertices, u32* indices) {
    render_command_t* self = (render_command_t*) malloc(sizeof(render_command_t));
    self->prev = NULL;
    self->next = NULL;
    memcpy(self->vertices, vertices, sizeof self->vertices);
    memcpy(self->indices, indices, sizeof self->indices);
    return self;
}

/// Frees the specified render command instance
void render_command_free(render_command_t* self) {
    free(self);
}

/// Creates a new render group
render_group_t* render_group_new(void) {
    render_group_t* self = (render_group_t*) malloc(sizeof(render_group_t));
    self->begin = NULL;
    self->end = NULL;
    self->commands = 0;
    self->mutex = mutex_new();

    vertex_array_create(&self->vertex_array);
    vertex_buffer_create(&self->vertex_buffer);
    index_buffer_create(&self->index_buffer);

    // attributes are usually `attrib_position`, `attrib_color`, `attrib_texture`
    static shader_type_t attributes[] = { FLOAT3, FLOAT3, FLOAT2 };
    static vertex_buffer_layout_t layout;
    layout.attributes = attributes;
    layout.count = STACK_ARRAY_SIZE(attributes);

    vertex_buffer_layout(&self->vertex_buffer, &layout);
    vertex_array_vertex_buffer(&self->vertex_array, &self->vertex_buffer);
    vertex_array_index_buffer(&self->vertex_array, &self->index_buffer);
    return self;
}

/// Clears the specified render group (i.e. deletes the commands)
void render_group_clear(render_group_t* self) {
    mutex_lock(self->mutex);
    render_command_t* it = self->begin;

    while (it != NULL) {
        render_command_t* tmp = it;
        it = it->next;
        render_command_free(tmp);
    }

    self->begin = NULL;
    self->end = NULL;
    self->commands = 0;
    mutex_unlock(self->mutex);
}

/// Frees the specified render group (i.e. delete the commands and free memory)
void render_group_free(render_group_t* self) {
    render_group_clear(self);
    mutex_free(self->mutex);
    index_buffer_destroy(&self->index_buffer);
    vertex_buffer_destroy(&self->vertex_buffer);
    vertex_array_destroy(&self->vertex_array);
    free(self);
}

/// Pushes a set of vertices and indices to the render group
void render_group_push(render_group_t* self, vertex_t* vertices, u32* indices) {
    while (self->commands >= RENDER_GROUP_COMMANDS_MAX)
        ;

    mutex_lock(self->mutex);
    render_command_t* command = render_command_new(vertices, indices);
    if (self->begin == NULL) {
        self->begin = command;
        self->commands++;
        mutex_unlock(self->mutex);
        return;
    }

    render_command_t* tmp = self->begin;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = command;
    command->prev = tmp;
    self->end = command;
    self->commands++;
    mutex_unlock(self->mutex);
}

/// Creates the post processing pipeline
void post_processing_create(post_processing_t* self) {
    frame_buffer_create(&self->frame, 800, 600);
    self->group = render_group_new();
    shader_create(&self->shader, "assets/processing_vertex.glsl", "assets/processing_fragment.glsl");
}

/// Destroys the post processing pipeline
void post_processing_destroy(post_processing_t* self) {
    shader_destroy(&self->shader);
    render_group_free(self->group);
    frame_buffer_destroy(&self->frame);
}

/// Submits an actual indexed OpenGL draw call to the GPU
static void renderer_draw_indexed(vertex_array_t* vertex_array, shader_t* shader, u32 mode) {
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
void renderer_clear_color(f32vec4_t* color) {
    glClearColor(color->x, color->y, color->z, color->w);
}

/// Creates a new renderer and initializes its pipeline
void renderer_create(renderer_t* self, const char* font) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_create(&self->glyph_shader, "assets/glyph_vertex.glsl", "assets/glyph_fragment.glsl");
    self->glyph_group = render_group_new();
    self->glyphs = glyph_cache_new(font);

    shader_create(&self->quad_shader, "assets/quad_vertex.glsl", "assets/quad_fragment.glsl");
    self->quad_group = render_group_new();
    post_processing_create(&self->post);
}

/// Destroys the specified renderer
void renderer_destroy(renderer_t* self) {
    shader_destroy(&self->glyph_shader);
    render_group_free(self->glyph_group);
    glyph_cache_free(self->glyphs);
    shader_destroy(&self->quad_shader);
    render_group_free(self->quad_group);
    post_processing_destroy(&self->post);
}

/// Begins a renderer batch by resetting all render groups
void renderer_begin_batch(renderer_t* self) {
    render_group_clear(self->glyph_group);
    render_group_clear(self->quad_group);
}

/// Submits the specified render group and issues an indexed draw call
static void render_group_submit(render_group_t* group, shader_t* shader) {
    if (group->commands == 0) {
        return;
    }

    mutex_lock(group->mutex);
    u32 vertices_size = QUAD_VERTICES * sizeof(vertex_t);
    u32 indices_size = QUAD_INDICES * sizeof(u32);
    vertex_t* vertices = (vertex_t*) malloc((u64) vertices_size * group->commands);
    u32* indices = (u32*) malloc((u64) indices_size * group->commands);

    u32 insert_index = 0;
    for (render_command_t* it = group->begin; it != NULL; it = it->next) {
        memcpy((u8*) vertices + (ptrdiff_t) (vertices_size * insert_index), it->vertices, vertices_size);
        memcpy((u8*) indices + (ptrdiff_t) (indices_size * insert_index), it->indices, indices_size);
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
void renderer_end_batch(renderer_t* self) {
    render_group_submit(self->quad_group, &self->quad_shader);

    texture_bind(&self->glyphs->atlas, 0);
    shader_uniform_sampler(&self->glyph_shader, "uniform_glyph_atlas", 0);
    render_group_submit(self->glyph_group, &self->glyph_shader);
}

/// Draws a quad at the given position
void renderer_draw_quad(renderer_t* self, f32vec2_t* position, f32vec2_t* size, f32vec3_t* color) {
    vertex_t vertices[] = { { { position->x, position->y, 0.0f }, *color, { 0.0f, 1.0f } },
                            { { position->x, position->y + size->y, 0.0f }, *color, { 0.0f, 0.0f } },
                            { { position->x + size->x, position->y + size->y, 0.0f }, *color, { 1.0f, 0.0f } },
                            { { position->x + size->x, position->y, 0.0f }, *color, { 1.0f, 1.0f } } };

    u32 index_offset = self->quad_group->commands * 4;
    u32 indices[] = { 0 + index_offset, 1 + index_offset, 2 + index_offset,
                      2 + index_offset, 0 + index_offset, 3 + index_offset };
    render_group_push(self->quad_group, vertices, indices);
}

/// Draws the specified symbol at the given position
void renderer_draw_symbol(renderer_t* self, glyph_info_t* symbol, f32vec2_t* position, f32vec3_t* color, f32 scale) {
    // clang-format off
    f32vec2_t scaled_size = { symbol->size.x * scale, symbol->size.y * scale };
    f32vec2_t scaled_position = {
        position->x + symbol->bearing.x * scale,
        position->y + (symbol->size.y - symbol->bearing.y) * scale
    };
    vertex_t vertices[] = {
        { { scaled_position.x, scaled_position.y, 0.0f }, *color, { symbol->texture_offset, 0.0f } },
        { { scaled_position.x, scaled_position.y + scaled_size.y, 0.0f }, *color, { symbol->texture_offset, symbol->texture_span.y } },
        { { scaled_position.x + scaled_size.x, scaled_position.y + scaled_size.y, 0.0f }, *color, { symbol->texture_offset + symbol->texture_span.x, symbol->texture_span.y } },
        { { scaled_position.x + scaled_size.x, scaled_position.y, 0.0f }, *color, { symbol->texture_offset + symbol->texture_span.x, 0.0f } }
    };
    // clang-format on

    u32 index_offset = self->glyph_group->commands * 4;
    u32 indices[] = { 0 + index_offset, 1 + index_offset, 2 + index_offset,
                      2 + index_offset, 0 + index_offset, 3 + index_offset };
    render_group_push(self->glyph_group, vertices, indices);
}

/// Draws the specified text at the given position
void renderer_draw_text(renderer_t* self, f32vec2_t* position, f32vec3_t* color, f32 scale, const char* fmt, ...) {
    char text_buffer[0x1000];
    va_list list;
    va_start(list, fmt);
    u32 length = (u32) vsnprintf(text_buffer, sizeof text_buffer, fmt, list);
    va_end(list);

    f32vec2_t position_iterator = *position;
    for (u32 i = 0; i < length; i++) {
        char symbol = text_buffer[i];
        if (symbol == '\n') {
            position_iterator.x = position->x;
            position_iterator.y += FONT_SIZE * scale;
        } else if (symbol == '\t') {
            glyph_info_t glyph_info;
            glyph_cache_acquire(self->glyphs, &glyph_info, ' ');
            for (u32 j = 0; j < 4; j++) {
                renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
                position_iterator.x += glyph_info.advance.x * scale;
            }
        } else {
            glyph_info_t glyph_info;
            glyph_cache_acquire(self->glyphs, &glyph_info, symbol);
            renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
            position_iterator.x += glyph_info.advance.x * scale;
        }
    }
    *position = position_iterator;
}

/// Draws an input indicator
static void renderer_draw_indicator(renderer_t* self, f32vec2_t* position, f32vec3_t* color, f32 scale) {
    // first we draw the input indicator
    glyph_info_t input_indicator_info;
    glyph_cache_acquire(self->glyphs, &input_indicator_info, ']');
    renderer_draw_symbol(self, &input_indicator_info, position, color, scale);
    position->x += input_indicator_info.advance.x * scale;
}

/// Draws the cursor and the specified text at the given position
void renderer_draw_text_with_cursor(renderer_t* self,
                                    f32vec2_t* position,
                                    f32vec3_t* color,
                                    f32 scale,
                                    u32 cursor_index,
                                    const char* fmt,
                                    ...) {
    char text_buffer[0x1000];
    va_list list;
    va_start(list, fmt);
    u32 length = (u32) vsnprintf(text_buffer, sizeof text_buffer, fmt, list);
    va_end(list);

    f32vec2_t position_iterator = *position;
    renderer_draw_indicator(self, &position_iterator, color, scale);

    // then we fetch the cursor glyph
    glyph_info_t cursor_info;
    glyph_cache_acquire(self->glyphs, &cursor_info, '_');
    if (cursor_index == 0) {
        renderer_draw_symbol(self, &cursor_info, &position_iterator, color, scale);
    }

    for (u32 i = 0; i < length; i++) {
        char symbol = text_buffer[i];
        if (symbol == '\t') {
            // if we encounter a tab, advance by four spaces
            glyph_info_t glyph_info;
            glyph_cache_acquire(self->glyphs, &glyph_info, ' ');
            for (u32 j = 0; j < 4; j++) {
                renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
                position_iterator.x += glyph_info.advance.x * scale;
            }
        } else if (symbol == '\n') {
            position_iterator.x = position->x;
            position_iterator.y += FONT_SIZE * scale;
            renderer_draw_indicator(self, &position_iterator, color, scale);
        } else {
            // any other character
            glyph_info_t glyph_info;
            glyph_cache_acquire(self->glyphs, &glyph_info, symbol);
            renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
            position_iterator.x += glyph_info.advance.x * scale;
        }

        if (i == cursor_index - 1) {
            renderer_draw_symbol(self, &cursor_info, &position_iterator, color, scale);
        }
    }
    *position = position_iterator;
}

/// Captures all following draw commands into a frame buffer
void renderer_begin_capture(renderer_t* self) {
    frame_buffer_bind(&self->post.frame);
}

/// Ends the capture of draw commands
void renderer_end_capture(renderer_t* self) {
    // unbind frame buffer in order to actually render stuff now
    frame_buffer_unbind();
    renderer_clear();
    render_group_clear(self->post.group);

    f32vec2_t size = { (f32) self->post.frame.width, (f32) self->post.frame.height };
    f32vec3_t color = { 1.0f, 1.0f, 1.0f };

    // clang-format off
    vertex_t vertices[] = {
        { .position = { 0.0f, 0.0f, 0.0f }, .color = color, { 0.0f, 1.0f } },
        { .position = { 0.0f, size.y, 0.0f }, .color = color, { 0.0f, 0.0f } },
        { .position = { size.x, size.y, 0.0f }, .color = color, { 1.0f, 0.0f } },
        { .position = { size.x, 0.0f, 0.0f }, .color = color, { 1.0f, 1.0f } }
    };
    // clang-format on
    u32 indices[] = { 0, 1, 2, 2, 0, 3 };

    render_group_push(self->post.group, vertices, indices);
    frame_buffer_bind_texture(&self->post.frame, 0);
    shader_uniform_sampler(&self->post.shader, "uniform_frame", 0);
    render_group_submit(self->post.group, &self->post.shader);
}
