//
// MIT License
//
// Copyright (c) 2024 Elias Engelbert Plank
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

#ifndef RETRO_GPU_BUFFER_H
#define RETRO_GPU_BUFFER_H

#include "types.h"

typedef enum shader_type {
    INT = 0,
    INT2,
    INT3,
    INT4,
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    SAMPLER = INT
} shader_type_t;

typedef struct vertex_buffer_layout {
    shader_type_t *attributes;
    u32 count;
} vertex_buffer_layout_t;

typedef struct vertex_buffer {
    u32 handle;
    vertex_buffer_layout_t *layout;
} vertex_buffer_t;

/// Creates a vertex buffer on the gpu
/// @param self The vertex buffer handle
void vertex_buffer_create(vertex_buffer_t *self);

/// Destroys the vertex buffer
/// @param self The vertex buffer Handle
void vertex_buffer_destroy(vertex_buffer_t *self);

/// Sets the data for the vertex buffer
/// @param self The vertex buffer handle
/// @param data A pointer to the first element of the data
/// @param size The size of the data in bytes
void vertex_buffer_data(vertex_buffer_t *self, const void *data, u32 size);

/// Sets the attribute layout for the specified buffer
/// @param self The vertex buffer handle
/// @param layout The vertex buffer layout handle
void vertex_buffer_layout(vertex_buffer_t *self, vertex_buffer_layout_t *layout);

/// Binds the vertex buffer
/// @param self The vertex buffer handle
void vertex_buffer_bind(vertex_buffer_t *self);

/// Unbinds the currently bound vertex buffer
void vertex_buffer_unbind(void);

typedef struct index_buffer {
    u32 handle;
    u32 count;
} index_buffer_t;

/// Creates an index buffer on the gpu
/// @param self The index buffer handle
void index_buffer_create(index_buffer_t *self);

/// Destroys the index buffer
/// @param self The index buffer handle
void index_buffer_destroy(index_buffer_t *self);

/// Sets the data for the specified index buffer
/// @param self The index buffer handle
/// @param data A pointer to the first index element
/// @param count The number of indices
void index_buffer_data(index_buffer_t *self, const u32 *data, u32 count);

/// Binds the specified buffer
/// @param self The index buffer handle
void index_buffer_bind(index_buffer_t *self);

/// Unbinds the currently bound index buffer
void index_buffer_unbind(void);

typedef struct vertex_array {
    u32 handle;
    vertex_buffer_t *vertex_buffer;
    index_buffer_t *index_buffer;
} vertex_array_t;

/// Creates a new vertex array
/// @param self The vertex array handle
void vertex_array_create(vertex_array_t *self);

/// Destroys the vertex array
/// @param self The vertex array handle
void vertex_array_destroy(vertex_array_t *self);

/// Sets the vertex buffer for the vertex array, this sets all the specified attributes
/// @param self The vertex array handle
/// @param vertex_buffer The vertex buffer handle
void vertex_array_vertex_buffer(vertex_array_t *self, vertex_buffer_t *vertex_buffer);

/// Sets the index buffer for the vertex array
/// @param self The vertex array handle
/// @param index_buffer The index buffer handle
void vertex_array_index_buffer(vertex_array_t *self, index_buffer_t *index_buffer);

/// Binds the vertex array
/// @param self The vertex array handle
void vertex_array_bind(vertex_array_t *self);

/// Unbinds the currently bound vertex array
void vertex_array_unbind(void);

typedef struct frame_buffer_specification {
    s32 width;
    s32 height;
    s32 internal_format;
    u32 pixel_type;
    u32 pixel_format;
} frame_buffer_specification_t;

typedef struct frame_buffer {
    u32 handle;
    u32 texture_handle;
    u32 render_handle;
    frame_buffer_specification_t spec;
} frame_buffer_t;

/// Creates a frame buffer of specified size
/// @param self The frame buffer handle
/// @param spec The frame buffer specification
/// @return A boolean value that indicates whether the frame buffer could be created
bool frame_buffer_create(frame_buffer_t *self, frame_buffer_specification_t *spec);

/// Destroys the frame buffer
/// @param self The frame buffer handle
void frame_buffer_destroy(frame_buffer_t *self);

/// Invalidates the frame buffer, this needs to be called whenever the frame buffer is resized
/// @param self The frame buffer handle
/// @return A boolean value that indicates whether the frame buffer could be invalidated
bool frame_buffer_invalidate(frame_buffer_t *self);

/// Resizes the frame buffer
/// @param self The frame buffer handle
/// @param width The new width
/// @param height The new height
/// @return A boolean value that indicates whether the frame buffer could be resized
bool frame_buffer_resize(frame_buffer_t *self, s32 width, s32 height);

/// Binds the specified frame buffer for rendering
/// @param self The frame buffer handle
void frame_buffer_bind(frame_buffer_t *self);

/// Binds the texture of the frame buffer at the specified sampler slot
/// @param self The frame buffer handle
/// @param slot The sampler slot
void frame_buffer_bind_texture(frame_buffer_t *self, u32 slot);

/// Unbinds the currently bound frame buffer
void frame_buffer_unbind(void);

#endif// RETRO_GPU_BUFFER_H
