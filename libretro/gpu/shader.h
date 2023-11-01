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

#ifndef RETRO_GPU_SHADER_H
#define RETRO_GPU_SHADER_H

#include "texture.h"
#include "util/utility.h"

typedef struct shader {
    u32 handle;
} shader_t;

/// Creates a shader from the given vertex and fragment shader files
/// @param self The shader handle
/// @param vertex The path to the vertex shader
/// @param fragment The path to the fragment shader
/// @return A boolean that indicates whether the shader could be created or not
bool shader_create(shader_t* self, const char* vertex, const char* fragment);

/// Destroys the specified shader
/// @param self The shader handle
void shader_destroy(shader_t* self);

/// Sets a sampler2D (texture) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param slot The sampler slot for the texture
void shader_uniform_sampler(shader_t* self, const char* name, u32 slot);

/// Sets an integer (s32) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_s32(shader_t* self, const char* name, s32 value);

/// Sets an 2D integer (s32vec2_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_s32vec2(shader_t* self, const char* name, s32vec2_t* value);

/// Sets an 3D integer (s32vec3_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_s32vec3(shader_t* self, const char* name, s32vec3_t* value);

/// Sets an 4D integer (s32vec4_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_s32vec4(shader_t* self, const char* name, s32vec4_t* value);

/// Sets a float (f32) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_f32(shader_t* self, const char* name, f32 value);

/// Sets an 2D float (f32vec2_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_f32vec2(shader_t* self, const char* name, f32vec2_t* value);

/// Sets an 3D float (f32vec3_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_f32vec3(shader_t* self, const char* name, f32vec3_t* value);

/// Sets an 4D float (f32vec4_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_f32vec4(shader_t* self, const char* name, f32vec4_t* value);

/// Sets an 4x4 matrix (f32mat4_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_f32mat4(shader_t* self, const char* name, f32mat4_t* value);

/// Binds the specified shader
/// @param self shader handle
void shader_bind(shader_t* self);

/// Unbinds the currently bound shader
void shader_unbind(void);

#endif// RETRO_GPU_SHADER_H
