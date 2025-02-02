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

#include <stdio.h>

#include "buffer.h"

/// Converts a shader type to its stride in bytes
static s32 shader_type_stride(ShaderType type) {
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

/// Converts a shader type to its OpenGL base type
static s32 shader_type_opengl(ShaderType type) {
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

/// Calculates the number of primitives in a shader type
static s32 shader_type_primitives(ShaderType type) {
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

/// Calculates the total stride of a vertex buffer layout
static s32 vertex_buffer_layout_stride(VertexBufferLayout *layout) {
    s32 stride = 0;
    for (u32 i = 0; i < layout->count; i++) {
        ShaderType attribute = *(layout->attributes + i);
        stride += shader_type_stride(attribute);
    }
    return stride;
}

/// Creates a vertex buffer on the gpu
void vertex_buffer_create(VertexBuffer *self) {
    self->handle = 0;
    self->layout = NULL;
    glGenBuffers(1, &self->handle);
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
}

/// Destroys the vertex buffer
void vertex_buffer_destroy(VertexBuffer *self) {
    glDeleteBuffers(1, &self->handle);
    self->layout = NULL;
}

/// Sets the data for the vertex buffer
void vertex_buffer_data(VertexBuffer *self, const void *data, u32 size) {
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

/// Sets the attribute layout for the specified buffer
void vertex_buffer_layout(VertexBuffer *self, VertexBufferLayout *layout) {
    self->layout = layout;
}

/// Binds the vertex buffer
void vertex_buffer_bind(VertexBuffer *self) {
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
}

/// Unbinds the currently bound vertex buffer
void vertex_buffer_unbind(void) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/// Creates an index buffer on the gpu
void index_buffer_create(IndexBuffer *self) {
    self->handle = 0;
    self->count = 0;
    glGenBuffers(1, &self->handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
}

/// Destroys the index buffer
void index_buffer_destroy(IndexBuffer *self) {
    glDeleteBuffers(1, &self->handle);
}

/// Sets the data for the specified index buffer
void index_buffer_data(IndexBuffer *self, const u32 *data, u32 count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (count * sizeof count), data, GL_DYNAMIC_DRAW);
    self->count = count;
}

/// Binds the specified buffer
void index_buffer_bind(IndexBuffer *self) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
}

/// Unbinds the currently bound index buffer
void index_buffer_unbind(void) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/// Creates a new vertex array
void vertex_array_create(VertexArray *self) {
    self->handle = 0;
    self->vertex_buffer = NULL;
    self->index_buffer = NULL;
    glGenVertexArrays(1, &self->handle);
    glBindVertexArray(self->handle);
}

/// Destroys the vertex array
void vertex_array_destroy(VertexArray *self) {
    glDeleteVertexArrays(1, &self->handle);
}

/// Sets the vertex buffer for the vertex array, this sets all the specified attributes
void vertex_array_vertex_buffer(VertexArray *self, VertexBuffer *vertex_buffer) {
    glBindVertexArray(self->handle);
    vertex_buffer_bind(vertex_buffer);

    s64 offset = 0;
    s32 stride = vertex_buffer_layout_stride(vertex_buffer->layout);
    for (u32 i = 0; i < vertex_buffer->layout->count; i++) {
        glEnableVertexAttribArray(i);
        ShaderType attribute = *(vertex_buffer->layout->attributes + i);
        GLenum opengl_type = (GLenum) shader_type_opengl(attribute);
        if (opengl_type == GL_FLOAT) {
            glVertexAttribPointer(i, shader_type_primitives(attribute), opengl_type, GL_FALSE, stride,
                                  (const void *) offset);
        } else if (opengl_type == GL_INT) {
            glVertexAttribIPointer(i, shader_type_primitives(attribute), opengl_type, stride, (const void *) offset);
        }
        offset += shader_type_stride(attribute);
    }
    self->vertex_buffer = vertex_buffer;
}

/// Sets the index buffer for the vertex array
void vertex_array_index_buffer(VertexArray *self, IndexBuffer *index_buffer) {
    glBindVertexArray(self->handle);
    index_buffer_bind(index_buffer);
    self->index_buffer = index_buffer;
}

/// Binds the vertex array
void vertex_array_bind(VertexArray *self) {
    glBindVertexArray(self->handle);
}

/// Unbinds the currently bound vertex array
void vertex_array_unbind(void) {
    glBindVertexArray(0);
}

/// Creates a frame buffer of specified size
bool frame_buffer_create(FrameBuffer *self, FrameBufferSpecification *spec) {
    self->handle = 0;
    self->texture_handle = 0;
    self->render_handle = 0;
    self->spec = *spec;
    return frame_buffer_invalidate(self);
}

/// Destroys the frame buffer
void frame_buffer_destroy(FrameBuffer *self) {
    glDeleteFramebuffers(1, &self->handle);
    glDeleteTextures(1, &self->texture_handle);
    glDeleteRenderbuffers(1, &self->render_handle);
}

/// Checks if the frame buffer is complete
static bool frame_buffer_is_valid(FrameBuffer *buffer) {
    frame_buffer_bind(buffer);
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

/// Invalidates the frame buffer, this needs to be called whenever the frame buffer is resized
bool frame_buffer_invalidate(FrameBuffer *self) {
    if (self->handle) {
        glDeleteFramebuffers(1, &self->handle);
        glDeleteTextures(1, &self->texture_handle);
        glDeleteRenderbuffers(1, &self->render_handle);
    }

    glGenFramebuffers(1, &self->handle);
    glBindFramebuffer(GL_FRAMEBUFFER, self->handle);

    glGenTextures(1, &self->texture_handle);
    glBindTexture(GL_TEXTURE_2D, self->texture_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, self->spec.internal_format, self->spec.width, self->spec.height, 0,
                 self->spec.pixel_format, self->spec.pixel_type, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->texture_handle, 0);

    glGenRenderbuffers(1, &self->render_handle);
    glBindRenderbuffer(GL_RENDERBUFFER, self->render_handle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, self->spec.width, self->spec.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, self->render_handle);

    if (!frame_buffer_is_valid(self)) {
        fprintf(stderr, "invalid frame buffer\n");
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

/// Resizes the frame buffer
bool frame_buffer_resize(FrameBuffer *self, s32 width, s32 height) {
    if (width <= 0 || height <= 0 || (width == self->spec.width && height == self->spec.height)) {
        return false;
    }
    self->spec.width = width;
    self->spec.height = height;
    return frame_buffer_invalidate(self);
}

/// Binds the specified frame buffer for rendering
void frame_buffer_bind(FrameBuffer *self) {
    glBindFramebuffer(GL_FRAMEBUFFER, self->handle);
    glViewport(0, 0, self->spec.width, self->spec.height);
}

/// Binds the texture of the frame buffer at the specified sampler slot
void frame_buffer_bind_texture(FrameBuffer *self, u32 slot) {
    glBindTextureUnit(slot, self->texture_handle);
}

/// Unbinds the currently bound frame buffer
void frame_buffer_unbind(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
