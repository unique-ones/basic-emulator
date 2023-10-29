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

#ifndef RETRO_BUFFER_H
#define RETRO_BUFFER_H

#include "types.h"

typedef enum shader_type { INT = 0, INT2, INT3, INT4, FLOAT, FLOAT2, FLOAT3, FLOAT4, SAMPLER = INT } shader_type_t;

typedef struct vertex_buffer_layout {
    shader_type_t* attributes;
    u32 count;
} vertex_buffer_layout_t;

typedef struct vertex_buffer {
    u32 handle;
    vertex_buffer_layout_t* layout;
} vertex_buffer_t;

/**
 * @brief creates a vertex buffer on the gpu
 * @param self buffer handle
 */
void vertex_buffer_create(vertex_buffer_t* self);

/**
 * @brief destroys the vertex buffer
 * @param self buffer Handle
 */
void vertex_buffer_destroy(vertex_buffer_t* self);

/**
 * @brief sets the data for the specified buffer
 * @param self self handle
 * @param data pointer to the first element of the data
 * @param size size in bytes
 */
void vertex_buffer_data(vertex_buffer_t* self, const void* data, u32 size);

/**
 * @brief sets the attribute layout for the specified buffer
 * @param self self handle
 * @param layout layout handle
 */
void vertex_buffer_layout(vertex_buffer_t* self, vertex_buffer_layout_t* layout);

/**
 * @brief binds the specified buffer
 * @param self Buffer handle
 */
void vertex_buffer_bind(vertex_buffer_t* self);

/**
 * @brief unbinds the currently bound buffer
 */
void vertex_buffer_unbind(void);

typedef struct index_buffer {
    u32 handle;
    u32 count;
} index_buffer_t;

/**
 * @brief creates an index buffer on the gpu
 * @param self Buffer handle
 */
void index_buffer_create(index_buffer_t* self);

/**
 * @brief destroys the index buffer
 * @param self Buffer Handle
 */
void index_buffer_destroy(index_buffer_t* self);

/**
 * @brief sets the data for the specified buffer
 * @param self buffer handle
 * @param data pointer to the first index
 * @param count number of indices
 */
void index_buffer_data(index_buffer_t* self, const u32* data, u32 count);

/**
 * @brief binds the specified buffer
 * @param self self handle
 */
void index_buffer_bind(index_buffer_t* self);

/**
 * @brief unbinds the currently bound buffer
 */
void index_buffer_unbind(void);

typedef struct vertex_array {
    u32 handle;
    vertex_buffer_t* vertex_buffer;
    index_buffer_t* index_buffer;
} vertex_array_t;

/**
 * @brief creates a new vertex array
 * @param self vertex array handle
 */
void vertex_array_create(vertex_array_t* self);

/**
 * @brief destroys the specified vertex array
 * @param self vertex array handle
 */
void vertex_array_destroy(vertex_array_t* self);

/**
 * @brief sets the vertex buffer for the vertex array, this sets all the specified attributes
 * @param self vertex array handle
 * @param vertex_buffer vertex buffer handle
 */
void vertex_array_vertex_buffer(vertex_array_t* self, vertex_buffer_t* vertex_buffer);

/**
 * @brief sets the index buffer for the vertex array
 * @param self vertex array handle
 * @param index_buffer index buffer handle
 */
void vertex_array_index_buffer(vertex_array_t* self, index_buffer_t* index_buffer);

/**
 * @brief binds the specified vertex array
 * @param self vertex array handle
 */
void vertex_array_bind(vertex_array_t* self);

/**
 * @brief unbinds the currently bound vertex array
 */
void vertex_array_unbind(void);

typedef struct frame_buffer {
    u32 handle;
    u32 texture_handle;
    u32 render_handle;
    s32 width;
    s32 height;
} frame_buffer_t;

/**
 * @brief creates a frame buffer of specified size
 * @param self frame buffer handle
 * @param width initial width
 * @param height initial height
 * @return bool
 */
bool frame_buffer_create(frame_buffer_t* self, s32 width, s32 height);

/**
 * @brief destroys the frame buffer
 * @param self frame buffer handle
 */
void frame_buffer_destroy(frame_buffer_t* self);

/**
 * @brief invalidates the frame buffer, this needs to be called if the frame buffer is resized
 * @param self frame buffer handle
 * @return bool
 */
bool frame_buffer_invalidate(frame_buffer_t* self);

/**
 * @brief resizes the frame buffer
 * @param self frame buffer handle
 * @param width new width
 * @param height new height
 * @return bool
 */
bool frame_buffer_resize(frame_buffer_t* self, s32 width, s32 height);

/**
 * @brief binds the specified frame buffer for rendering
 * @param self frame buffer handle
 */
void frame_buffer_bind(frame_buffer_t* self);

/**
 * @brief binds the texture of the frame buffer at the specified sampler slot
 * @param self frame buffer handle
 * @param slot sampler slot
 */
void frame_buffer_bind_texture(frame_buffer_t* self, u32 slot);

/**
 * @brief unbinds the currently bound frame buffer
 */
void frame_buffer_unbind(void);

#endif// RETRO_BUFFER_H
