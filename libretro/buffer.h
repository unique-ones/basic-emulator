#ifndef RETRO_BUFFER_H
#define RETRO_BUFFER_H

#include "types.h"

#include <glad/glad.h>

typedef enum shader_type {
    INT = 0, INT2, INT3, INT4, FLOAT, FLOAT2, FLOAT3, FLOAT4, SAMPLER = INT
} shader_type_t;

typedef struct vertex_buffer_layout {
    shader_type_t* attributes;
    u32 count;
} vertex_buffer_layout_t;

typedef struct vertex_buffer {
    u32 handle;
    vertex_buffer_layout_t* layout;
} vertex_buffer_t;

void vertex_buffer_create(vertex_buffer_t* buffer);
void vertex_buffer_destroy(vertex_buffer_t* buffer);
void vertex_buffer_data(vertex_buffer_t* buffer, const void* data, u32 size);
void vertex_buffer_layout(vertex_buffer_t* buffer, vertex_buffer_layout_t* layout);
void vertex_buffer_bind(vertex_buffer_t* buffer);
void vertex_buffer_unbind();

#endif // RETRO_BUFFER_H