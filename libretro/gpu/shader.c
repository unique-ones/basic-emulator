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

#include "shader.h"

/// Compiles the specified shader type from source
static u32 shader_compile(binary_buffer_t* source, u32 type) {
    u32 program = glCreateShader(type);
    const GLchar* shader_source = source->data;
    glShaderSource(program, 1, &shader_source, NULL);
    glCompileShader(program);

    s32 success;
    glGetShaderiv(program, GL_COMPILE_STATUS, &success);
    if (!success) {
        s32 info_length;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &info_length);

        binary_buffer_t failure_info;
        failure_info.data = (char*) malloc((size_t) info_length);
        failure_info.size = (u32) info_length;
        glGetShaderInfoLog(program, info_length, &info_length, failure_info.data);
        glDeleteProgram(program);
        fprintf(stderr, "compilation failed: %s\n", failure_info.data);
        free(failure_info.data);
        failure_info.size = 0;
        return 0;
    }
    return program;
}

/// Creates a shader from the given vertex and fragment shader files
bool shader_create(shader_t* self, const char* vertex, const char* fragment) {
    binary_buffer_t vertex_source, fragment_source;
    if (!file_read(&vertex_source, vertex) || !file_read(&fragment_source, fragment)) {
        free(vertex_source.data);
        free(fragment_source.data);
        vertex_source.size = 0;
        fragment_source.size = 0;
        return false;
    }

    u32 vertex_program = shader_compile(&vertex_source, GL_VERTEX_SHADER);
    u32 fragment_program = shader_compile(&fragment_source, GL_FRAGMENT_SHADER);
    free(vertex_source.data);
    free(fragment_source.data);
    vertex_source.size = 0;
    fragment_source.size = 0;

    u32 handle = glCreateProgram();
    glAttachShader(handle, vertex_program);
    glAttachShader(handle, fragment_program);
    glLinkProgram(handle);

    s32 link_success;
    glGetProgramiv(handle, GL_LINK_STATUS, &link_success);
    if (!link_success) {
        s32 info_length;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &info_length);

        binary_buffer_t failure_info;
        failure_info.data = (char*) malloc((size_t) info_length);
        failure_info.size = (u32) info_length;
        glGetProgramInfoLog(handle, info_length, NULL, failure_info.data);
        glDeleteProgram(handle);
        glDeleteProgram(vertex_program);
        glDeleteProgram(fragment_program);
        fprintf(stderr, "self linking failed: %.*s\n", failure_info.size, failure_info.data);
        free(failure_info.data);
        failure_info.size = 0;
        return false;
    }

    s32 uniform_count;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &uniform_count);

    s32 uniform_length;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_length);

    if (uniform_count > 0 && uniform_length > 0) {
        binary_buffer_t uniform_name;
        uniform_name.data = (char*) malloc(uniform_length);
        uniform_name.size = (u32) uniform_length;

        for (u32 i = 0; i < (u32) uniform_count; i++) {
            s32 length, size;
            u32 data_type;

            glGetActiveUniform(handle, i, uniform_length, &length, &size, &data_type, uniform_name.data);
            s32 location = glGetUniformLocation(handle, uniform_name.data);
            fprintf(stderr, "self [%s, %s] uniform %.*s has location %d\n", vertex, fragment, uniform_name.size,
                    uniform_name.data, location);
        }
        free(uniform_name.data);
        uniform_name.size = 0;
    }
    self->handle = handle;
    return true;
}

/// Destroys the specified shader
void shader_destroy(shader_t* self) {
    glDeleteProgram(self->handle);
}

/// Sets a sampler2D (texture) uniform
void shader_uniform_sampler(shader_t* self, const char* name, u32 slot) {
    shader_uniform_s32(self, name, (s32) slot);
}

/// Sets an integer (s32) uniform
void shader_uniform_s32(shader_t* self, const char* name, s32 value) {
    glUseProgram(self->handle);
    glUniform1i(glGetUniformLocation(self->handle, name), value);
}

/// Sets an 2D integer (s32vec2_t) uniform
void shader_uniform_s32vec2(shader_t* self, const char* name, s32vec2_t* value) {
    glUseProgram(self->handle);
    glUniform2i(glGetUniformLocation(self->handle, name), value->x, value->y);
}

/// Sets an 3D integer (s32vec3_t) uniform
void shader_uniform_s32vec3(shader_t* self, const char* name, s32vec3_t* value) {
    glUseProgram(self->handle);
    glUniform3i(glGetUniformLocation(self->handle, name), value->x, value->y, value->z);
}

/// Sets an 4D integer (s32vec4_t) uniform
void shader_uniform_s32vec4(shader_t* self, const char* name, s32vec4_t* value) {
    glUseProgram(self->handle);
    glUniform4i(glGetUniformLocation(self->handle, name), value->x, value->y, value->z, value->w);
}

/// Sets a float (f32) uniform
void shader_uniform_f32(shader_t* self, const char* name, f32 value) {
    glUseProgram(self->handle);
    glUniform1f(glGetUniformLocation(self->handle, name), value);
}

/// Sets an 2D float (f32vec2_t) uniform
void shader_uniform_f32vec2(shader_t* self, const char* name, f32vec2_t* value) {
    glUseProgram(self->handle);
    glUniform2f(glGetUniformLocation(self->handle, name), value->x, value->y);
}

void shader_uniform_f32vec3(shader_t* self, const char* name, f32vec3_t* value) {
    glUseProgram(self->handle);
    glUniform3f(glGetUniformLocation(self->handle, name), value->x, value->y, value->z);
}

/// Sets an 4D float (f32vec4_t) uniform
void shader_uniform_f32vec4(shader_t* self, const char* name, f32vec4_t* value) {
    glUseProgram(self->handle);
    glUniform4f(glGetUniformLocation(self->handle, name), value->x, value->y, value->z, value->w);
}

/// Sets an 4x4 matrix (f32mat4_t) uniform
void shader_uniform_f32mat4(shader_t* self, const char* name, f32mat4_t* value) {
    glUseProgram(self->handle);
    glUniformMatrix4fv(glGetUniformLocation(self->handle, name), 1, GL_FALSE, &value->value[0].x);
}

/// Binds the specified shader
void shader_bind(shader_t* self) {
    glUseProgram(self->handle);
}

/// Unbinds the currently bound shader
void shader_unbind(void) {
    glUseProgram(0);
}
