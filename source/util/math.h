// Copyright (c) 2025 Elias Engelbert Plank

#ifndef RETRO_UTIL_MATH_H
#define RETRO_UTIL_MATH_H

/// Returns the greater of the two passed variables
/// @param a The first variable
/// @param b The second variable
/// @return The max value
s32 s32_max(s32 a, s32 b);

/// Returns the smaller of the two passed variables
/// @param a The first variable
/// @param b The second variable
/// @return The min value
s32 s32_min(s32 a, s32 b);

/// Returns the greater of the two passed variables
/// @param a The first variable
/// @param b The second variable
/// @return The max value
u32 u32_max(u32 a, u32 b);

/// Returns the smaller of the two passed variables
/// @param a The first variable
/// @param b The second variable
/// @return The min value
u32 u32_min(u32 a, u32 b);

/// Clamps the specified value to the given bounds
/// @param n The value that shall be clamped
/// @param min The lower bound
/// @param max The upper bound
/// @return The clamped value
s32 s32_clamp(s32 n, s32 min, s32 max);

/// Returns the greater of the two passed variables
/// @param a The first variable
/// @param b The second variable
/// @return The max value
s64 s64_max(s64 a, s64 b);

/// Returns the smaller of the two passed variables
/// @param a The first variable
/// @param b The second variable
/// @return The min value
s64 s64_min(s64 a, s64 b);

/// Clamps the specified value to the given bounds
/// @param n The value that shall be clamped
/// @param min The lower bound
/// @param max The upper bound
/// @return The clamped value
s64 s64_clamp(s64 n, s64 min, s64 max);

/// Creates an identity matrix
/// @param self The matrix handle
void f32mat4_create_identity(F32Mat4 *self);

/// Creates an orthogonal projection matrix
/// @param self The matrix handle
/// @param left The left coordinate of the orthogonal frustum
/// @param right The right coordinate of the orthogonal frustum
/// @param bottom The bottom coordinate of the orthogonal frustum
/// @param top The top coordinate of the orthogonal frustum
void f32mat4_create_orthogonal(F32Mat4 *self, f32 left, f32 right, f32 bottom, f32 top);

#endif// RETRO_UTIL_MATH_H
