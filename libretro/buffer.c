#include "buffer.h"

#include <stdio.h>

static s32 shader_type_stride(shader_type_t type) {
    switch (type) {
        case INT:
            return sizeof(GLint);
        case INT2:
            return 2 * sizeof(GLint);
        case INT3:
            return 3 * sizeof(GLint);
        case INT4:
            return 4 * sizeof(GLint);
        case FLOAT:
            return sizeof(float);
        case FLOAT2:
            return 2 * sizeof(float);
        case FLOAT3:
            return 3 * sizeof(float);
        case FLOAT4:
            return 4 * sizeof(float);
        default:
            return 0;
    }
}

static s32 shader_type_opengl(shader_type_t type) {
    switch (type) {
        case INT:
        case INT2:
        case INT3:
        case INT4:
            return GL_INT;
        case FLOAT:
        case FLOAT2:
        case FLOAT3:
        case FLOAT4:
            return GL_FLOAT;
        default:
            return 0;
    }
}

static s32 shader_type_primitives(shader_type_t type) {
    switch (type) {
        case INT:
            return 1;
        case INT2:
            return 2;
        case INT3:
            return 3;
        case INT4:
            return 4;
        case FLOAT:
            return 1;
        case FLOAT2:
            return 2;
        case FLOAT3:
            return 3;
        case FLOAT4:
            return 4;
        default:
            return 0;
    }
}

static s32 vertex_buffer_layout_stride(vertex_buffer_layout_t* layout) {
    s32 stride = 0;
    for (u32 i = 0; i < layout->count; i++) {
        shader_type_t attribute = *(layout->attributes + i);
        stride += shader_type_stride(attribute);
    }
    return stride;
}

static bool frame_buffer_is_valid(frame_buffer_t* buffer) {
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void vertex_buffer_create(vertex_buffer_t* buffer) {
    buffer->handle = 0;
    buffer->layout = NULL;
    glGenBuffers(1, &buffer->handle);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->handle);
}

void vertex_buffer_destroy(vertex_buffer_t* buffer) {
    glDeleteBuffers(1, &buffer->handle);
    buffer->layout = NULL;
}

void vertex_buffer_data(vertex_buffer_t* buffer, const void* data, u32 size) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->handle);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void vertex_buffer_layout(vertex_buffer_t* buffer, vertex_buffer_layout_t* layout) {
    buffer->layout = layout;
}

void vertex_buffer_bind(vertex_buffer_t* buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer->handle);
}

void vertex_buffer_unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void index_buffer_create(index_buffer_t* buffer) {
    buffer->handle = 0;
    buffer->count = 0;
    glGenBuffers(1, &buffer->handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->handle);
}
void index_buffer_destroy(index_buffer_t* buffer) {
    glDeleteBuffers(1, &buffer->handle);
}

void index_buffer_data(index_buffer_t* buffer, const u32* data, u32 count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (count * sizeof count), data, GL_DYNAMIC_DRAW);
    buffer->count = count;
}

void index_buffer_bind(index_buffer_t* buffer) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->handle);
}

void index_buffer_unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void vertex_array_create(vertex_array_t* vertex_array) {
    vertex_array->handle = 0;
    vertex_array->vertex_buffer = NULL;
    vertex_array->index_buffer = NULL;
    glGenVertexArrays(1, &vertex_array->handle);
    glBindVertexArray(vertex_array->handle);
}

void vertex_array_destroy(vertex_array_t* vertex_array) {
    glDeleteVertexArrays(1, &vertex_array->handle);
}

void vertex_array_vertex_buffer(vertex_array_t* vertex_array, vertex_buffer_t* vertex_buffer) {
    glBindVertexArray(vertex_array->handle);
    vertex_buffer_bind(vertex_buffer);

    s64 offset = 0;
    s32 stride = vertex_buffer_layout_stride(vertex_buffer->layout);
    for (s32 i = 0; i < vertex_buffer->layout->count; i++) {
        glEnableVertexAttribArray(i);
        shader_type_t attribute = *(vertex_buffer->layout->attributes + i);
        s32 opengl_type = shader_type_opengl(attribute);
        if (opengl_type == GL_FLOAT) {
            glVertexAttribPointer(i, shader_type_primitives(attribute), opengl_type, GL_FALSE, stride,
                                  (const void*) offset);
        } else if (opengl_type == GL_INT) {
            glVertexAttribIPointer(i, shader_type_primitives(attribute), opengl_type, stride, (const void*) offset);
        }
        offset += shader_type_stride(attribute);
    }
    vertex_array->vertex_buffer = vertex_buffer;
}

void vertex_array_index_buffer(vertex_array_t* vertex_array, index_buffer_t* index_buffer) {
    glBindVertexArray(vertex_array->handle);
    index_buffer_bind(index_buffer);
    vertex_array->index_buffer = index_buffer;
}

void vertex_array_bind(vertex_array_t* vertex_array) {
    glBindVertexArray(vertex_array->handle);
}

void vertex_array_unbind() {
    glBindVertexArray(0);
}

bool frame_buffer_create(frame_buffer_t* buffer, s32 width, s32 height) {
    buffer->handle = 0;
    buffer->texture_handle = 0;
    buffer->render_handle = 0;
    buffer->width = width;
    buffer->height = height;
    return frame_buffer_invalidate(buffer);
}

bool frame_buffer_invalidate(frame_buffer_t* buffer) {
    if (buffer->handle) {
        glDeleteFramebuffers(1, &buffer->handle);
        glDeleteTextures(1, &buffer->handle);
        glDeleteRenderbuffers(1, &buffer->handle);
    }

    glGenFramebuffers(1, &buffer->handle);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer->handle);

    glGenTextures(1, &buffer->texture_handle);
    glBindTexture(GL_TEXTURE_2D, buffer->texture_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buffer->width, buffer->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer->texture_handle, 0);

    glGenRenderbuffers(1, &buffer->render_handle);
    glBindRenderbuffer(GL_RENDERBUFFER, buffer->render_handle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, buffer->width, buffer->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->render_handle);

    if (!frame_buffer_is_valid(buffer)) {
        fprintf(stderr, "invalid frame buffer\n");
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

bool frame_buffer_resize(frame_buffer_t* buffer, s32 width, s32 height) {
    if (width <= 0 || height <= 0) {
        return false;
    }
    buffer->width = width;
    buffer->height = height;
    return frame_buffer_invalidate(buffer);
}

void frame_buffer_bind(frame_buffer_t* buffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, buffer->handle);
    glViewport(0, 0, buffer->width, buffer->height);
}

void frame_buffer_bind_texture(frame_buffer_t* buffer, u32 slot) {
    glBindTextureUnit(slot, buffer->texture_handle);
}

void frame_buffer_unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}