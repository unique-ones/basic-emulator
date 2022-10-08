#ifndef RETRO_MATH_H
#define RETRO_MATH_H

#include "types.h"

#include <string.h>

/**
 * @brief Returns the greater of the two passed variables
 * @param a First variable
 * @param b Second variable
 */
s32 max_s32(s32 a, s32 b);

/**
 * @brief Returns the greater of the two passed variables
 * @param a First variable
 * @param b Second variable
 */
f32 max_f32(f32 a, f32 b);

/**
 * @brief Creates an identity matrix
 * @param matrix Matrix handle
 */
void f32mat4_create_identity(f32mat4_t* matrix);

/**
 * @brief Creates an orthogonal projection matrix
 * @param matrix Matrix handle
 * @param left Left coordinate of the orthogonal frustum
 * @param right Right coordinate of the orthogonal frustum
 * @param bottom Bottom coordinate of the orthogonal frustum
 * @param top Top coordinate of the orthogonal frustum
 */
void f32mat4_create_ortho(f32mat4_t* matrix, f32 left, f32 right, f32 bottom, f32 top);

#endif// RETRO_MATH_H