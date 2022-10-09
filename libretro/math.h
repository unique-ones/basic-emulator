#ifndef RETRO_MATH_H
#define RETRO_MATH_H

#include "types.h"

#include <string.h>

/**
 * @brief Returns the greater of the two passed variables
 * @param a First variable
 * @param b Second variable
 * @return Max value
 */
s32 s32_max(s32 a, s32 b);

/**
 * @brief Returns the smaller of the two passed variables
 * @param a First variable
 * @param b Second variable
 * @return Min value
 */
s32 s32_min(s32 a, s32 b);

/**
 * @brief Clamps the specified value to the given bounds
 * @param n Value that shall be clamped
 * @param min Lower bound
 * @param max Upper bound
 * @return Clamped value
 */
s32 s32_clamp(s32 n, s32 min, s32 max);

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