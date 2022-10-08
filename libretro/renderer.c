#include "renderer.h"

/**
 * @brief Indexed draw-call
 * @param vertex_array Vertex array that holds the vertex and index buffers
 * @param shader Shader handle
 * @param mode Draw mode
 */
static void renderer_draw_indexed(vertex_array_t* vertex_array, shader_t* shader, u32 mode) {
    shader_bind(shader);
    vertex_array_bind(vertex_array);
    glDrawElements(mode, (s32) vertex_array->index_buffer->count, GL_UNSIGNED_INT, NULL);
}

void renderer_clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_clear_color(const f32vec4_t* color) {
    glClearColor(color->x, color->y, color->z, color->w);
}

void renderer_create(renderer_t* renderer, shader_t* shader) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    vertex_array_create(&renderer->vertex_array);
    vertex_buffer_create(&renderer->vertex_buffer);
    index_buffer_create(&renderer->index_buffer);
    memset(renderer->vertices, 0, sizeof renderer->vertices);
    memset(renderer->indices, 0, sizeof renderer->indices);
    renderer->batch_size = 0;

    // Attributes are `attrib_position`, `attrib_color`, `attrib_texture`
    shader_type_t attributes[] = { FLOAT3, FLOAT3, FLOAT2 };
    vertex_buffer_layout_t layout;
    layout.attributes = attributes;
    layout.count = STACK_ARRAY_SIZE(attributes);

    vertex_buffer_layout(&renderer->vertex_buffer, &layout);
    vertex_array_vertex_buffer(&renderer->vertex_array, &renderer->vertex_buffer);
    vertex_array_index_buffer(&renderer->vertex_array, &renderer->index_buffer);
    renderer->shader = *shader;
}

void renderer_destroy(renderer_t* renderer) {
    index_buffer_destroy(&renderer->index_buffer);
    vertex_buffer_destroy(&renderer->vertex_buffer);
    vertex_array_destroy(&renderer->vertex_array);
    shader_destroy(&renderer->shader);
}

void renderer_begin_batch(renderer_t* renderer) {
    renderer->batch_size = 0;
}

void renderer_end_batch(renderer_t* renderer) {
    vertex_buffer_data(&renderer->vertex_buffer, renderer->vertices, renderer->batch_size * 4 * sizeof(vertex_t));
    index_buffer_data(&renderer->index_buffer, renderer->indices, renderer->batch_size * 6);
    renderer_draw_indexed(&renderer->vertex_array, &renderer->shader, GL_TRIANGLES);
}

void renderer_draw_quad(renderer_t* renderer,
                        const f32vec2_t* position,
                        const f32vec2_t* size,
                        const f32vec3_t* color) {
    // In case the batch gets to big, we submit the draw call and immediately start a new batch again
    if (renderer->batch_size >= MAX_BATCH_SIZE) {
        renderer_end_batch(renderer);
        renderer_begin_batch(renderer);
    }

    const vertex_t vertices[] = { { { position->x, position->y, 0.0f }, *color, { 0.0f, 1.0f } },
                                  { { position->x, position->y + size->y, 0.0f }, *color, { 0.0f, 0.0f } },
                                  { { position->x + size->x, position->y + size->y, 0.0f }, *color, { 1.0f, 0.0f } },
                                  { { position->x + size->x, position->y, 0.0f }, *color, { 1.0f, 1.0f } } };

    const u32 index_offset = renderer->batch_size * 4;
    const u32 indices[] = { 0 + index_offset, 1 + index_offset, 2 + index_offset,
                            2 + index_offset, 0 + index_offset, 3 + index_offset };
    memcpy(renderer->vertices + renderer->batch_size * STACK_ARRAY_SIZE(vertices), vertices, sizeof vertices);
    memcpy(renderer->indices + renderer->batch_size * STACK_ARRAY_SIZE(indices), indices, sizeof indices);
    renderer->batch_size++;
}

bool text_renderer_create(text_renderer_t* text_renderer, const char* path) {
    shader_t glyph_shader;
    if (!shader_create(&glyph_shader, "assets/glyph_vertex.glsl", "assets/glyph_fragment.glsl")) {
        return false;
    }
    if (!glyph_cache_create(&text_renderer->cache, path)) {
        return false;
    }
    renderer_create(&text_renderer->renderer, &glyph_shader);
    return true;
}

void text_renderer_draw_symbol(text_renderer_t* text_renderer,
                               glyph_info_t* symbol,
                               const f32vec2_t* position,
                               const f32vec3_t* color,
                               f32 scale) {
    // In case the batch gets to big, we submit the draw call and immediately start a new batch again
    renderer_t* renderer = &text_renderer->renderer;
    if (renderer->batch_size >= MAX_BATCH_SIZE) {
        renderer_end_batch(renderer);
        renderer_begin_batch(renderer);
    }

    f32vec2_t scaled_position = { position->x + symbol->bearing.x * scale,
                                  position->y + (symbol->size.y - symbol->bearing.y) * scale };
    f32vec2_t scaled_size = { symbol->size.x * scale, symbol->size.y * scale };
    vertex_t vertices[] = { { .position = { scaled_position.x, scaled_position.y },
                              .color = *color,
                              .texture = { symbol->texture_offset, 0.0f } },
                            { .position = { scaled_position.x, scaled_position.y + scaled_size.y },
                              .color = *color,
                              .texture = { symbol->texture_offset, symbol->texture_span.y } },
                            { .position = { scaled_position.x + scaled_size.x, scaled_position.y + scaled_size.y },
                              .color = *color,
                              .texture = { symbol->texture_offset + symbol->texture_span.x, symbol->texture_span.y } },
                            { .position = { scaled_position.x + scaled_size.x, scaled_position.y },
                              .color = *color,
                              .texture = { symbol->texture_offset + symbol->texture_span.x, 0.0f } } };

    const u32 index_offset = renderer->batch_size * 4;
    const u32 indices[] = { 0 + index_offset, 1 + index_offset, 2 + index_offset,
                            2 + index_offset, 0 + index_offset, 3 + index_offset };
    memcpy(text_renderer->renderer.vertices + renderer->batch_size * STACK_ARRAY_SIZE(vertices), vertices,
           sizeof vertices);
    memcpy(text_renderer->renderer.indices + renderer->batch_size * STACK_ARRAY_SIZE(indices), indices, sizeof indices);
    renderer->batch_size++;
}

void text_renderer_draw_text(text_renderer_t* text_renderer,
                             const char* text,
                             const f32vec2_t* position,
                             const f32vec3_t* color,
                             f32 scale) {
    renderer_t* renderer = &text_renderer->renderer;
    f32vec2_t position_iterator = *position;
    renderer_begin_batch(renderer);
    for (u32 i = 0; i < strlen(text); i++) {
        char symbol = text[i];
        if (symbol == '\n') {
            position_iterator.x = position->x;
            position_iterator.y += FONT_SIZE * scale;
            continue;
        }
        glyph_info_t glyph_info;
        glyph_cache_acquire(&text_renderer->cache, &glyph_info, symbol);
        text_renderer_draw_symbol(text_renderer, &glyph_info, &position_iterator, color, scale);
        position_iterator.x += glyph_info.advance.x * scale;
    }
    texture_bind(&text_renderer->cache.atlas, 0);
    shader_uniform_sampler(&renderer->shader, "uniform_glyph_atlas", 0);
    renderer_end_batch(renderer);
}