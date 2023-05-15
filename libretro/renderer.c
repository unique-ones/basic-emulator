#include <stdarg.h>
#include <stdio.h>

#include "arch/time.h"
#include "renderer.h"

render_command_t* render_command_new(vertex_t* vertices, u32* indices) {
    render_command_t* self = (render_command_t*) malloc(sizeof(render_command_t));
    self->prev = NULL;
    self->next = NULL;
    memcpy(self->vertices, vertices, sizeof self->vertices);
    memcpy(self->indices, indices, sizeof self->indices);
    return self;
}

void render_command_free(render_command_t* self) {
    free(self);
}

render_group_t* render_group_new(void) {
    render_group_t* self = (render_group_t*) malloc(sizeof(render_group_t));
    self->begin = NULL;
    self->end = NULL;
    self->commands = 0;
    self->mutex = mutex_new();
    return self;
}

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

void render_group_free(render_group_t* self) {
    render_group_clear(self);
    mutex_free(self->mutex);
    free(self);
}

void render_group_push(render_group_t* self, vertex_t* vertices, u32* indices) {
    while (self->commands >= RENDER_GROUP_COMMANDS_MAX);

    mutex_lock(self->mutex);
    render_command_t* command = render_command_new(vertices, indices);
    if (self->begin == NULL) {
        self->begin = command;
        command->prev = NULL;
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

static void renderer_draw_indexed(vertex_array_t* vertex_array, shader_t* shader, u32 mode) {
    shader_bind(shader);
    vertex_array_bind(vertex_array);
    glDrawElements(mode, (s32) vertex_array->index_buffer->count, GL_UNSIGNED_INT, NULL);
}

void renderer_clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_clear_color(f32vec4_t* color) {
    glClearColor(color->x, color->y, color->z, color->w);
}

void renderer_create(renderer_t* self, const char* font) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vertex_array_create(&self->vertex_array);
    vertex_buffer_create(&self->vertex_buffer);
    index_buffer_create(&self->index_buffer);

    // attributes are usually `attrib_position`, `attrib_color`, `attrib_texture`
    shader_type_t attributes[] = { FLOAT3, FLOAT3, FLOAT2 };
    vertex_buffer_layout_t layout;
    layout.attributes = attributes;
    layout.count = STACK_ARRAY_SIZE(attributes);

    vertex_buffer_layout(&self->vertex_buffer, &layout);
    vertex_array_vertex_buffer(&self->vertex_array, &self->vertex_buffer);
    vertex_array_index_buffer(&self->vertex_array, &self->index_buffer);

    shader_create(&self->glyph_shader, "assets/glyph_vertex.glsl", "assets/glyph_fragment.glsl");
    self->glyph_group = render_group_new();
    self->glyphs = glyph_cache_new(font);

    shader_create(&self->quad_shader, "assets/quad_vertex.glsl", "assets/quad_fragment.glsl");
    self->quad_group = render_group_new();
}

void renderer_destroy(renderer_t* self) {
    index_buffer_destroy(&self->index_buffer);
    vertex_buffer_destroy(&self->vertex_buffer);
    vertex_array_destroy(&self->vertex_array);

    shader_destroy(&self->glyph_shader);
    render_group_free(self->glyph_group);
    glyph_cache_free(self->glyphs);

    shader_destroy(&self->quad_shader);
    render_group_free(self->quad_group);
}

void renderer_begin_batch(renderer_t* self) {
    if (self->glyph_group->commands > 0 || self->quad_group->commands > 0) {
        renderer_end_batch(self);
    }
    render_group_clear(self->glyph_group);
    render_group_clear(self->quad_group);
}

static void renderer_group_end_batch(renderer_t* self, render_group_t* group) {
    mutex_lock(group->mutex);
    vertex_t* vertices = (vertex_t*) malloc(4 * sizeof(vertex_t) * group->commands);
    u32* indices = (u32*) malloc(6 * sizeof(u32) * group->commands);

    u32 insert_index = 0;
    for (render_command_t* it = group->begin; it != NULL; it = it->next) {
        memcpy((u8*) vertices + 4 * sizeof(vertex_t) * insert_index, it->vertices, sizeof it->vertices);
        memcpy((u8*) indices + 6 * sizeof(u32) * insert_index, it->indices, sizeof it->indices);
        insert_index++;
    }

    vertex_buffer_data(&self->vertex_buffer, vertices, group->commands * 4 * sizeof(vertex_t));
    free(vertices);

    index_buffer_data(&self->index_buffer, indices, group->commands * 6);
    free(indices);

    renderer_draw_indexed(&self->vertex_array, &self->glyph_shader, GL_TRIANGLES);
    mutex_unlock(group->mutex);
}

void renderer_end_batch(renderer_t* self) {
    renderer_group_end_batch(self, self->quad_group);

    texture_bind(&self->glyphs->atlas, 0);
    shader_uniform_sampler(&self->glyph_shader, "uniform_glyph_atlas", 0);
    renderer_group_end_batch(self, self->glyph_group);
}

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

static void renderer_draw_indicator(renderer_t* self, f32vec2_t* position, f32vec3_t* color, f32 scale) {
    // first we draw the input indicator
    glyph_info_t input_indicator_info;
    glyph_cache_acquire(self->glyphs, &input_indicator_info, ']');
    renderer_draw_symbol(self, &input_indicator_info, position, color, scale);
    position->x += input_indicator_info.advance.x * scale;
}

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
    renderer_begin_batch(self);
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

    texture_bind(&self->glyphs->atlas, 0);
    shader_uniform_sampler(&self->glyph_shader, "uniform_glyph_atlas", 0);
    renderer_end_batch(self);
    *position = position_iterator;
}
