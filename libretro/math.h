#ifndef RETRO_MATH_H
#define RETRO_MATH_H

#include <string.h>

#include "types.h"

/**
 * @brief returns the greater of the two passed variables
 * @param a first variable
 * @param b second variable
 * @return max value
 */
s32 s32_max(s32 a, s32 b);

/**
 * @brief returns the smaller of the two passed variables
 * @param a first variable
 * @param b second variable
 * @return min value
 */
s32 s32_min(s32 a, s32 b);

/**
 * @brief clamps the specified value to the given bounds
 * @param n value that shall be clamped
 * @param min lower bound
 * @param max upper bound
 * @return clamped value
 */
s32 s32_clamp(s32 n, s32 min, s32 max);

/**
 * @brief creates an identity matrix
 * @param self matrix handle
 */
void f32mat4_create_identity(f32mat4_t* self);

/**
 * @brief creates an orthogonal projection matrix
 * @param self matrix handle
 * @param left left coordinate of the orthogonal frustum
 * @param right right coordinate of the orthogonal frustum
 * @param bottom bottom coordinate of the orthogonal frustum
 * @param top top coordinate of the orthogonal frustum
 */
void f32mat4_create_orthogonal(f32mat4_t* self, f32 left, f32 right, f32 bottom, f32 top);

#endif// RETRO_MATH_H