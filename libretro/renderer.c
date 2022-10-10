#include "renderer.h"

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

void renderer_create(renderer_t* self, shader_t* shader) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vertex_array_create(&self->vertex_array);
    vertex_buffer_create(&self->vertex_buffer);
    index_buffer_create(&self->index_buffer);
    self->batch_size = 0;

    memset(self->vertices, 0, sizeof self->vertices);
    memset(self->indices, 0, sizeof self->indices);

    // attributes are usually `attrib_position`, `attrib_color`, `attrib_texture`
    shader_type_t attributes[] = { FLOAT3, FLOAT3, FLOAT2 };
    vertex_buffer_layout_t layout;
    layout.attributes = attributes;
    layout.count = STACK_ARRAY_SIZE(attributes);

    vertex_buffer_layout(&self->vertex_buffer, &layout);
    vertex_array_vertex_buffer(&self->vertex_array, &self->vertex_buffer);
    vertex_array_index_buffer(&self->vertex_array, &self->index_buffer);
    self->shader = *shader;
}

void renderer_destroy(renderer_t* self) {
    index_buffer_destroy(&self->index_buffer);
    vertex_buffer_destroy(&self->vertex_buffer);
    vertex_array_destroy(&self->vertex_array);
    shader_destroy(&self->shader);
}

void renderer_begin_batch(renderer_t* self) {
    self->batch_size = 0;
}

void renderer_end_batch(renderer_t* self) {
    vertex_buffer_data(&self->vertex_buffer, self->vertices, self->batch_size * 4 * sizeof(vertex_t));
    index_buffer_data(&self->index_buffer, self->indices, self->batch_size * 6);
    renderer_draw_indexed(&self->vertex_array, &self->shader, GL_TRIANGLES);
}

void renderer_draw_quad(renderer_t* self, f32vec2_t* position, f32vec2_t* size, f32vec3_t* color) {
    // in case the batch gets to big, we submit the draw call and immediately start a new batch again
    if (self->batch_size >= BATCH_CAPACITY) {
        renderer_end_batch(self);
        renderer_begin_batch(self);
    }

    vertex_t vertices[] = { { { position->x, position->y, 0.0f }, *color, { 0.0f, 1.0f } },
                            { { position->x, position->y + size->y, 0.0f }, *color, { 0.0f, 0.0f } },
                            { { position->x + size->x, position->y + size->y, 0.0f }, *color, { 1.0f, 0.0f } },
                            { { position->x + size->x, position->y, 0.0f }, *color, { 1.0f, 1.0f } } };

    u32 index_offset = self->batch_size * 4;
    u32 indices[] = { 0 + index_offset, 1 + index_offset, 2 + index_offset,
                      2 + index_offset, 0 + index_offset, 3 + index_offset };
    memcpy(self->vertices + self->batch_size * STACK_ARRAY_SIZE(vertices), vertices, sizeof vertices);
    memcpy(self->indices + self->batch_size * STACK_ARRAY_SIZE(indices), indices, sizeof indices);
    self->batch_size++;
}

bool text_renderer_create(text_renderer_t* self, const char* path) {
    shader_t glyph_shader;
    if (!shader_create(&glyph_shader, "assets/glyph_vertex.glsl", "assets/glyph_fragment.glsl")) {
        return false;
    }
    if (!glyph_cache_create(&self->cache, path)) {
        return false;
    }
    renderer_create(&self->renderer, &glyph_shader);
    return true;
}

void text_renderer_destroy(text_renderer_t* self) {
    renderer_destroy(&self->renderer);
    glyph_cache_destroy(&self->cache);
}

void text_renderer_draw_symbol(text_renderer_t* self,
                               glyph_info_t* symbol,
                               f32vec2_t* position,
                               f32vec3_t* color,
                               f32 scale) {
    // in case the batch gets to big, we submit the draw call and immediately start a new batch again
    renderer_t* renderer = &self->renderer;
    if (renderer->batch_size >= BATCH_CAPACITY) {
        renderer_end_batch(renderer);
        renderer_begin_batch(renderer);
    }

    // clang-format off
    f32vec2_t scaled_size = { symbol->size.x * scale, symbol->size.y * scale };
    f32vec2_t scaled_position = {
        position->x + symbol->bearing.x * scale,
        position->y + (symbol->size.y - symbol->bearing.y) * scale
    };
    vertex_t vertices[] = {
        { { scaled_position.x, scaled_position.y }, *color, { symbol->texture_offset, 0.0f } },
        { { scaled_position.x, scaled_position.y + scaled_size.y }, *color, { symbol->texture_offset, symbol->texture_span.y } },
        { { scaled_position.x + scaled_size.x, scaled_position.y + scaled_size.y }, *color, { symbol->texture_offset + symbol->texture_span.x, symbol->texture_span.y } },
        { { scaled_position.x + scaled_size.x, scaled_position.y }, *color, { symbol->texture_offset + symbol->texture_span.x, 0.0f } }
    };
    // clang-format on

    u32 index_offset = renderer->batch_size * 4;
    u32 indices[] = { 0 + index_offset, 1 + index_offset, 2 + index_offset,
                      2 + index_offset, 0 + index_offset, 3 + index_offset };
    memcpy(self->renderer.vertices + renderer->batch_size * STACK_ARRAY_SIZE(vertices), vertices, sizeof vertices);
    memcpy(self->renderer.indices + renderer->batch_size * STACK_ARRAY_SIZE(indices), indices, sizeof indices);
    renderer->batch_size++;
}

void text_renderer_draw_text(text_renderer_t* self,
                             const char* text,
                             u32 length,
                             f32vec2_t* position,
                             f32vec3_t* color,
                             f32 scale) {
    renderer_t* renderer = &self->renderer;
    f32vec2_t position_iterator = *position;
    renderer_begin_batch(renderer);
    for (u32 i = 0; i < length; i++) {
        char symbol = text[i];
        if (symbol == '\n') {
            position_iterator.x = position->x;
            position_iterator.y += FONT_SIZE * scale;
        } else if (symbol == '\t') {
            glyph_info_t glyph_info;
            glyph_cache_acquire(&self->cache, &glyph_info, ' ');
            for (u32 j = 0; j < 4; j++) {
                text_renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
                position_iterator.x += glyph_info.advance.x * scale;
            }
        } else {
            glyph_info_t glyph_info;
            glyph_cache_acquire(&self->cache, &glyph_info, symbol);
            text_renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
            position_iterator.x += glyph_info.advance.x * scale;
        }
    }
    texture_bind(&self->cache.atlas, 0);
    shader_uniform_sampler(&renderer->shader, "uniform_glyph_atlas", 0);
    renderer_end_batch(renderer);
}

void text_renderer_draw_input(text_renderer_t* self,
                              input_buffer_t* input,
                              f32vec2_t* position,
                              f32vec3_t* color,
                              f32 scale) {
    renderer_t* renderer = &self->renderer;
    f32vec2_t position_iterator = *position;
    renderer_begin_batch(renderer);

    // first we draw the input indicator
    glyph_info_t input_indicator_info;
    glyph_cache_acquire(&self->cache, &input_indicator_info, ']');
    text_renderer_draw_symbol(self, &input_indicator_info, &position_iterator, color, scale);
    position_iterator.x += input_indicator_info.advance.x * scale;

    // then we fetch the cursor glyph
    glyph_info_t cursor_info;
    glyph_cache_acquire(&self->cache, &cursor_info, '_');
    if (input->cursor == 0) {
        text_renderer_draw_symbol(self, &cursor_info, &position_iterator, color, scale);
    }

    for (u32 i = 0; i < input->fill; i++) {
        char symbol = input->data[i];
        if (symbol == '\t') {
            // if we encounter a tab, advance by four spaces
            glyph_info_t glyph_info;
            glyph_cache_acquire(&self->cache, &glyph_info, ' ');
            for (u32 j = 0; j < 4; j++) {
                text_renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
                position_iterator.x += glyph_info.advance.x * scale;
            }
        } else {
            // any other character
            glyph_info_t glyph_info;
            glyph_cache_acquire(&self->cache, &glyph_info, symbol);
            text_renderer_draw_symbol(self, &glyph_info, &position_iterator, color, scale);
            position_iterator.x += glyph_info.advance.x * scale;
        }

        if (i == input->cursor - 1) {
            text_renderer_draw_symbol(self, &cursor_info, &position_iterator, color, scale);
        }
    }

    texture_bind(&self->cache.atlas, 0);
    shader_uniform_sampler(&renderer->shader, "uniform_glyph_atlas", 0);
    renderer_end_batch(renderer);
}