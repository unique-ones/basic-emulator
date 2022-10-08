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
 * @brief Creates a vertex buffer on the gpu
 * @param buffer Buffer handle
 */
void vertex_buffer_create(vertex_buffer_t* buffer);

/**
 * @brief Destroys the vertex buffer
 * @param buffer Buffer Handle
 */
void vertex_buffer_destroy(vertex_buffer_t* buffer);

/**
 * @brief Sets the data for the specified buffer
 * @param buffer Buffer handle
 * @param data Pointer to the first element of the data
 * @param size Size in bytes
 */
void vertex_buffer_data(vertex_buffer_t* buffer, const void* data, u32 size);

/**
 * @brief Sets the attribute layout for the specified buffer
 * @param buffer Buffer handle
 * @param layout Layout handle
 */
void vertex_buffer_layout(vertex_buffer_t* buffer, vertex_buffer_layout_t* layout);

/**
 * @brief Binds the specified buffer
 * @param buffer Buffer handle
 */
void vertex_buffer_bind(vertex_buffer_t* buffer);

/**
 * @brief Unbinds the currently bound buffer
 */
void vertex_buffer_unbind();

typedef struct index_buffer {
    u32 handle;
    u32 count;
} index_buffer_t;

/**
 * @brief Creates a index buffer on the gpu
 * @param buffer Buffer handle
 */
void index_buffer_create(index_buffer_t* buffer);

/**
 * @brief Destroys the index buffer
 * @param buffer Buffer Handle
 */
void index_buffer_destroy(index_buffer_t* buffer);

/**
 * @brief Sets the data for the specified buffer
 * @param buffer Buffer handle
 * @param data Pointer to the first index
 * @param count Number of indices
 */
void index_buffer_data(index_buffer_t* buffer, const u32* data, u32 count);

/**
 * @brief Binds the specified buffer
 * @param buffer Buffer handle
 */
void index_buffer_bind(index_buffer_t* buffer);

/**
 * @brief Unbinds the currently bound buffer
 */
void index_buffer_unbind();

typedef struct vertex_array {
    u32 handle;
    vertex_buffer_t* vertex_buffer;
    index_buffer_t* index_buffer;
} vertex_array_t;

/**
 * @brief Creates a new vertex array
 * @param vertex_array Vertex array handle
 */
void vertex_array_create(vertex_array_t* vertex_array);

/**
 * @brief Destroys the specified vertex array
 * @param vertex_array Vertex array handle
 */
void vertex_array_destroy(vertex_array_t* vertex_array);

/**
 * @brief Sets the vertex buffer for the vertex array, this sets all the specified attributes
 * @param vertex_array Vertex array handle
 * @param vertex_buffer Vertex buffer handle
 */
void vertex_array_vertex_buffer(vertex_array_t* vertex_array, vertex_buffer_t* vertex_buffer);

/**
 * @brief Sets the index buffer for the vertex array
 * @param vertex_array Vertex array handle
 * @param index_buffer Index buffer handle
 */
void vertex_array_index_buffer(vertex_array_t* vertex_array, index_buffer_t* index_buffer);

/**
 * @brief Binds the specified vertex array
 * @param vertex_array Vertex array handle
 */
void vertex_array_bind(vertex_array_t* vertex_array);

/**
 * @brief Unbinds the currently bound vertex array
 */
void vertex_array_unbind();

typedef struct frame_buffer {
    u32 handle;
    u32 texture_handle;
    u32 render_handle;
    s32 width;
    s32 height;
} frame_buffer_t;

/**
 * @brief Creates a frame buffer of specified size
 * @param buffer Frame buffer handle
 * @param width Initial width
 * @param height Initial height
 * @return bool
 */
bool frame_buffer_create(frame_buffer_t* buffer, s32 width, s32 height);

/**
 * @brief Invalidates the frame buffer, this needs to be called if the frame buffer is resized
 * @param buffer Frame buffer handle
 * @return bool
 */
bool frame_buffer_invalidate(frame_buffer_t* buffer);

/**
 * @brief Resizes the frame buffer
 * @param buffer Frame buffer handle
 * @param width New width
 * @param height New height
 * @return bool
 */
bool frame_buffer_resize(frame_buffer_t* buffer, s32 width, s32 height);

/**
 * @brief Binds the specified frame buffer for rendering
 * @param buffer Frame buffer handle
 */
void frame_buffer_bind(frame_buffer_t* buffer);

/**
 * @brief Binds the texture of the frame buffer at the specified sampler slot
 * @param buffer Frame buffer handle
 * @param slot Sampler slot
 */
void frame_buffer_bind_texture(frame_buffer_t* buffer, u32 slot);

/**
 * @brief Unbinds the currently bound frame buffer
 */
void frame_buffer_unbind();

#endif// RETRO_BUFFER_H
