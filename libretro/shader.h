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

#ifndef RETRO_SHADER_H
#define RETRO_SHADER_H

#include "texture.h"
#include "utility.h"

typedef struct shader {
    u32 handle;
} shader_t;

/**
 * @brief creates a shader from the given vertex and fragment shader files
 * @param self shader handle
 * @param vertex path to the vertex shader
 * @param fragment path to the fragment shader
 * @return bool
 */
bool shader_create(shader_t* self, const char* vertex, const char* fragment);

/**
 * @brief destroys the specified shader
 * @param self shader handle
 */
void shader_destroy(shader_t* self);

/**
 * @brief sets a sampler2d (texture) uniform
 * @param self shader handle
 * @param name uniform name
 * @param slot sampler slot for the texture
 */
void shader_uniform_sampler(shader_t* self, const char* name, u32 slot);

/**
 * @brief sets an integer (s32) uniform
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_s32(shader_t* self, const char* name, s32 value);

/**
 * @brief sets a 2d-integer (s32vec2_t) uniform
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_s32vec2(shader_t* self, const char* name, s32vec2_t* value);

/**
 * @brief sets a 3d-integer (s32vec3_t) uniform
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_s32vec3(shader_t* self, const char* name, s32vec3_t* value);

/**
 * @brief sets a 4d-integer (s32vec4_t) uniform
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_s32vec4(shader_t* self, const char* name, s32vec4_t* value);

/**
 * @brief sets a float (f32) uniform
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_f32(shader_t* self, const char* name, f32 value);

/**
 * @brief sets a 2d-float (f32vec2_t) uniform
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_f32vec2(shader_t* self, const char* name, f32vec2_t* value);

/**
 * @brief sets a 3d-float (f32vec3_t) uniform
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_f32vec3(shader_t* self, const char* name, f32vec3_t* value);

/**
 * @brief sets a 4d-float (f32vec4_t) uniform
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_f32vec4(shader_t* self, const char* name, f32vec4_t* value);

/**
 * @brief sets a mat4 (f32mat4_t) uniform
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_f32mat4(shader_t* self, const char* name, f32mat4_t* value);

/**
 * @brief binds the specified shader
 * @param self shader handle
 */
void shader_bind(shader_t* self);

/**
 * @brief unbinds the currently bound shader
 */
void shader_unbind(void);

#endif// RETRO_SHADER_H
