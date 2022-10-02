#include "buffer.h"

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