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

#include <stdio.h>

#include "buffer.h"

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

void vertex_buffer_create(vertex_buffer_t* self) {
    self->handle = 0;
    self->layout = NULL;
    glGenBuffers(1, &self->handle);
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
}

void vertex_buffer_destroy(vertex_buffer_t* self) {
    glDeleteBuffers(1, &self->handle);
    self->layout = NULL;
}

void vertex_buffer_data(vertex_buffer_t* self, const void* data, u32 size) {
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void vertex_buffer_layout(vertex_buffer_t* self, vertex_buffer_layout_t* layout) {
    self->layout = layout;
}

void vertex_buffer_bind(vertex_buffer_t* self) {
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
}

void vertex_buffer_unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void index_buffer_create(index_buffer_t* self) {
    self->handle = 0;
    self->count = 0;
    glGenBuffers(1, &self->handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
}
void index_buffer_destroy(index_buffer_t* self) {
    glDeleteBuffers(1, &self->handle);
}

void index_buffer_data(index_buffer_t* self, const u32* data, u32 count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (count * sizeof count), data, GL_DYNAMIC_DRAW);
    self->count = count;
}

void index_buffer_bind(index_buffer_t* self) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
}

void index_buffer_unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void vertex_array_create(vertex_array_t* self) {
    self->handle = 0;
    self->vertex_buffer = NULL;
    self->index_buffer = NULL;
    glGenVertexArrays(1, &self->handle);
    glBindVertexArray(self->handle);
}

void vertex_array_destroy(vertex_array_t* self) {
    glDeleteVertexArrays(1, &self->handle);
}

void vertex_array_vertex_buffer(vertex_array_t* self, vertex_buffer_t* vertex_buffer) {
    glBindVertexArray(self->handle);
    vertex_buffer_bind(vertex_buffer);

    s64 offset = 0;
    s32 stride = vertex_buffer_layout_stride(vertex_buffer->layout);
    for (u32 i = 0; i < vertex_buffer->layout->count; i++) {
        glEnableVertexAttribArray(i);
        shader_type_t attribute = *(vertex_buffer->layout->attributes + i);
        GLenum opengl_type = (GLenum) shader_type_opengl(attribute);
        if (opengl_type == GL_FLOAT) {
            glVertexAttribPointer(i, shader_type_primitives(attribute), opengl_type, GL_FALSE, stride,
                                  (const void*) offset);
        } else if (opengl_type == GL_INT) {
            glVertexAttribIPointer(i, shader_type_primitives(attribute), opengl_type, stride, (const void*) offset);
        }
        offset += shader_type_stride(attribute);
    }
    self->vertex_buffer = vertex_buffer;
}

void vertex_array_index_buffer(vertex_array_t* self, index_buffer_t* index_buffer) {
    glBindVertexArray(self->handle);
    index_buffer_bind(index_buffer);
    self->index_buffer = index_buffer;
}

void vertex_array_bind(vertex_array_t* self) {
    glBindVertexArray(self->handle);
}

void vertex_array_unbind() {
    glBindVertexArray(0);
}

bool frame_buffer_create(frame_buffer_t* self, s32 width, s32 height) {
    self->handle = 0;
    self->texture_handle = 0;
    self->render_handle = 0;
    self->width = width;
    self->height = height;
    return frame_buffer_invalidate(self);
}

void frame_buffer_destroy(frame_buffer_t* self) {
    glDeleteFramebuffers(1, &self->handle);
    glDeleteTextures(1, &self->texture_handle);
    glDeleteRenderbuffers(1, &self->render_handle);
}

static bool frame_buffer_is_valid(frame_buffer_t* buffer) {
    frame_buffer_bind(buffer);
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

bool frame_buffer_invalidate(frame_buffer_t* self) {
    if (self->handle) {
        glDeleteFramebuffers(1, &self->handle);
        glDeleteTextures(1, &self->texture_handle);
        glDeleteRenderbuffers(1, &self->render_handle);
    }

    glGenFramebuffers(1, &self->handle);
    glBindFramebuffer(GL_FRAMEBUFFER, self->handle);

    glGenTextures(1, &self->texture_handle);
    glBindTexture(GL_TEXTURE_2D, self->texture_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, self->width, self->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->texture_handle, 0);

    glGenRenderbuffers(1, &self->render_handle);
    glBindRenderbuffer(GL_RENDERBUFFER, self->render_handle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, self->width, self->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, self->render_handle);

    if (!frame_buffer_is_valid(self)) {
        fprintf(stderr, "invalid frame self\n");
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

bool frame_buffer_resize(frame_buffer_t* self, s32 width, s32 height) {
    if (width <= 0 || height <= 0 || (width == self->width && height == self->height)) {
        return false;
    }
    self->width = width;
    self->height = height;
    return frame_buffer_invalidate(self);
}

void frame_buffer_bind(frame_buffer_t* self) {
    glBindFramebuffer(GL_FRAMEBUFFER, self->handle);
    glViewport(0, 0, self->width, self->height);
}

void frame_buffer_bind_texture(frame_buffer_t* self, u32 slot) {
    glBindTextureUnit(slot, self->texture_handle);
}

void frame_buffer_unbind(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
