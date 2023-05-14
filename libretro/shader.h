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
