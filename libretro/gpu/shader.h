// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_GPU_SHADER_H
#define RETRO_GPU_SHADER_H

#include "texture.h"
#include "util/utility.h"

typedef struct Shader {
    u32 handle;
} Shader;

/// Creates a shader from the given vertex and fragment shader files
/// @param self The shader handle
/// @param vertex The path to the vertex shader
/// @param fragment The path to the fragment shader
/// @return A boolean that indicates whether the shader could be created or not
bool shader_create(Shader *self, const char *vertex, const char *fragment);

/// Destroys the specified shader
/// @param self The shader handle
void shader_destroy(Shader *self);

/// Sets a sampler2D (texture) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param slot The sampler slot for the texture
void shader_uniform_sampler(Shader *self, const char *name, u32 slot);

/// Sets an integer (s32) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_s32(Shader *self, const char *name, s32 value);

/// Sets an 2D integer (s32vec2_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_s32vec2(Shader *self, const char *name, S32Vector2 *value);

/// Sets an 3D integer (s32vec3_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_s32vec3(Shader *self, const char *name, S32Vector3 *value);

/// Sets an 4D integer (s32vec4_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_s32vec4(Shader *self, const char *name, S32Vector4 *value);

/// Sets a float (f32) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_f32(Shader *self, const char *name, f32 value);

/// Sets an 2D float (f32vec2_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_f32vec2(Shader *self, const char *name, F32Vector2 *value);

/// Sets an 3D float (f32vec3_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_f32vec3(Shader *self, const char *name, F32Vector3 *value);

/// Sets an 4D float (f32vec4_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_f32vec4(Shader *self, const char *name, F32Vector4 *value);

/// Sets an 4x4 matrix (f32mat4_t) uniform
/// @param self The shader handle
/// @param name The uniform name
/// @param value The uniform value
void shader_uniform_f32mat4(Shader *self, const char *name, F32Mat4 *value);

/// Binds the specified shader
/// @param self shader handle
void shader_bind(Shader *self);

/// Unbinds the currently bound shader
void shader_unbind(void);

#endif// RETRO_GPU_SHADER_H
