#ifndef RETRO_SHADER_H
#define RETRO_SHADER_H

#include "texture.h"
#include "utility.h"


typedef struct shader {
    u32 handle;
} shader_t;

/**
 * @brief Creates a shader from the given vertex and fragment shader files
 * @param shader Shader handle
 * @param vertex Path to the vertex shader
 * @param fragment Path to the fragment shader
 * @return bool
 */
bool shader_create(shader_t* shader, const char* vertex, const char* fragment);

/**
 * @brief Destroys the specified shader
 * @param shader Shader handle
 */
void shader_destroy(shader_t* shader);

/**
 * @brief Sets a sampler2D (texture) uniform
 * @param shader Shader handle
 * @param name Uniform name
 * @param slot Sampler slot for the texture
 */
void shader_uniform_sampler(shader_t* shader, const char* name, u32 slot);

/**
 * @brief Sets an integer (s32) uniform
 * @param shader Shader handle
 * @param name Uniform name
 * @param value Value
 */
void shader_uniform_s32(shader_t* shader, const char* name, s32 value);

/**
 * @brief Sets an 2d-integer (s32vec2_t) uniform
 * @param shader Shader handle
 * @param name Uniform name
 * @param value Value
 */
void shader_uniform_s32vec2(shader_t* shader, const char* name, s32vec2_t* value);

/**
 * @brief Sets an 3d-integer (s32vec3_t) uniform
 * @param shader Shader handle
 * @param name Uniform name
 * @param value Value
 */
void shader_uniform_s32vec3(shader_t* shader, const char* name, s32vec3_t* value);

/**
 * @brief Sets an 4d-integer (s32vec4_t) uniform
 * @param shader Shader handle
 * @param name Uniform name
 * @param value Value
 */
void shader_uniform_s32vec4(shader_t* shader, const char* name, s32vec4_t* value);

/**
 * @brief Sets a float (f32) uniform
 * @param shader Shader handle
 * @param name Uniform name
 * @param value Value
 */
void shader_uniform_f32(shader_t* shader, const char* name, f32 value);

/**
 * @brief Sets a 2d-float (f32vec2_t) uniform
 * @param shader Shader handle
 * @param name Uniform name
 * @param value Value
 */
void shader_uniform_f32vec2(shader_t* shader, const char* name, f32vec2_t* value);

/**
 * @brief Sets a 3d-float (f32vec3_t) uniform
 * @param shader Shader handle
 * @param name Uniform name
 * @param value Value
 */
void shader_uniform_f32vec3(shader_t* shader, const char* name, f32vec3_t* value);

/**
 * @brief Sets a 4d-float (f32vec4_t) uniform
 * @param shader Shader handle
 * @param name Uniform name
 * @param value Value
 */
void shader_uniform_f32vec4(shader_t* shader, const char* name, f32vec4_t* value);

/**
 * @brief Sets a mat4 (f32mat4_t) uniform
 * @param shader Shader handle
 * @param name Uniform name
 * @param value Value
 */
void shader_uniform_f32mat4(shader_t* shader, const char* name, f32mat4_t* value);

/**
 * @brief Binds the specified shader
 * @param shader Shader handle
 */
void shader_bind(shader_t* shader);

/**
 * @brief Unbinds the currently bound shader
 */
void shader_unbind(void);

#endif// RETRO_SHADER_H
