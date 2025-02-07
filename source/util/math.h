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

#ifndef RETRO_UTIL_MATH_H
#define RETRO_UTIL_MATH_H

#include "core/types.h"

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
